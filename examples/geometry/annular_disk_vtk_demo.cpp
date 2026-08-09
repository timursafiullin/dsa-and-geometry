#include "geometry/io/vtk.h"
#include "geometry/primitives/triangle_mesh.h"
#include "geometry/topology/boundary.h"
#include "geometry/topology/half_edge_topology.h"
#include "geometry/topology/orientation.h"
#include "geometry/topology/triangle_topology.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace dsa::geometry;

namespace
{

    constexpr std::size_t kSegments = 16;
    constexpr double kOuterRadius = 2.0;
    constexpr double kInnerRadius = 0.75;
    constexpr double kPi = 3.14159265358979323846;

    TriangleMesh makeAnnularDisk()
    {
        TriangleMesh mesh;

        for (std::size_t index = 0; index < kSegments; ++index)
        {
            const double angle = 2.0 * kPi * index / kSegments;
            const double x = std::cos(angle);
            const double y = std::sin(angle);

            mesh.addVertex(Point3{kOuterRadius * x, kOuterRadius * y, 0.0});
        }

        for (std::size_t index = 0; index < kSegments; ++index)
        {
            const double angle = 2.0 * kPi * index / kSegments;
            const double x = std::cos(angle);
            const double y = std::sin(angle);

            mesh.addVertex(Point3{kInnerRadius * x, kInnerRadius * y, 0.0});
        }

        for (std::size_t index = 0; index < kSegments; ++index)
        {
            const VertexId outer = index;
            const VertexId nextOuter = (index + 1) % kSegments;
            const VertexId inner = kSegments + index;
            const VertexId nextInner = kSegments + (index + 1) % kSegments;

            mesh.addTriangle({outer, nextOuter, nextInner});
            mesh.addTriangle({outer, nextInner, inner});
        }

        return mesh;
    }

    double signedDoubleArea(
        const TriangleMesh& mesh,
        const std::vector<VertexId>& vertices
    )
    {
        double result = 0.0;

        for (std::size_t index = 0; index < vertices.size(); ++index)
        {
            const Point3& first = mesh.vertex(vertices[index]);
            const Point3& second = mesh.vertex(
                vertices[(index + 1) % vertices.size()]
            );

            result += first.x() * second.y() - first.y() * second.x();
        }

        return result;
    }

    void verifyAnnularDisk(const TriangleMesh& mesh)
    {
        assert(mesh.isValid());
        assert(mesh.isGeometricallyValid());
        assert(mesh.vertexCount() == 2 * kSegments);
        assert(mesh.triangleCount() == 2 * kSegments);

        const TriangleTopology topology = buildTriangleTopology(mesh);
        assert(topology.isManifold());
        assert(topology.vertexCount() == 2 * kSegments);
        assert(topology.triangleCount() == 2 * kSegments);
        assert(topology.edgeCount() == 4 * kSegments);
        assert(topology.vertexCount() - topology.edgeCount() + topology.triangleCount() == 0);

        const OrientationAnalysis orientation = analyzeOrientation(mesh, topology);
        assert(orientation.orientable);
        assert(orientation.consistentlyOriented);
        assert(orientation.connectedComponents == 1);
        assert(std::all_of(
            orientation.flipTriangles.begin(),
            orientation.flipTriangles.end(),
            [](std::uint8_t flip) { return flip == 0; }
        ));
        assert(isConsistentlyOriented(mesh, topology));
        assert(!isClosed(topology));
        assert(signedVolume(mesh) == 0.0);

        TriangleMesh reversed = mesh;
        reverseOrientation(reversed);
        const TriangleTopology reversedTopology = buildTriangleTopology(reversed);
        assert(isConsistentlyOriented(reversed, reversedTopology));

        const std::vector<EdgeId> edges = boundaryEdges(topology);
        const std::vector<VertexId> vertices = boundaryVertices(topology);

        assert(edges.size() == 2 * kSegments);
        assert(vertices.size() == 2 * kSegments);

        for (EdgeId edgeId = 0; edgeId < topology.edgeCount(); ++edgeId)
        {
            if (topology.isBoundaryEdge(edgeId))
                assert(topology.edgeIncidence(edgeId).triangles.size() == 1);
            else
                assert(topology.isInteriorEdge(edgeId));
        }

        const HalfEdgeTopology halfEdges = buildHalfEdgeTopology(mesh, topology);
        assert(halfEdges.halfEdgeCount() == 6 * kSegments);

        std::size_t boundaryHalfEdgeCount = 0;

        for (HalfEdgeId halfEdge = 0;
             halfEdge < halfEdges.halfEdgeCount();
             ++halfEdge)
        {
            if (halfEdges.isBoundary(halfEdge))
            {
                ++boundaryHalfEdgeCount;
                continue;
            }

            const HalfEdgeId twin = halfEdges.twin(halfEdge);
            assert(halfEdges.twin(twin) == halfEdge);
            assert(halfEdges.origin(mesh, halfEdge) ==
                   halfEdges.destination(mesh, twin));
            assert(halfEdges.destination(mesh, halfEdge) ==
                   halfEdges.origin(mesh, twin));
        }

        assert(boundaryHalfEdgeCount == 2 * kSegments);

        const std::vector<BoundaryLoop> loops = extractBoundaryLoops(mesh, halfEdges);
        assert(loops.size() == 2);

        const auto outerLoop = std::find_if(
            loops.begin(),
            loops.end(),
            [](const BoundaryLoop& loop)
            {
                return std::all_of(
                    loop.vertices.begin(),
                    loop.vertices.end(),
                    [](VertexId vertexId) { return vertexId < kSegments; }
                );
            }
        );

        const auto innerLoop = std::find_if(
            loops.begin(),
            loops.end(),
            [](const BoundaryLoop& loop)
            {
                return std::all_of(
                    loop.vertices.begin(),
                    loop.vertices.end(),
                    [](VertexId vertexId) { return vertexId >= kSegments; }
                );
            }
        );

        assert(outerLoop != loops.end());
        assert(innerLoop != loops.end());

        for (const BoundaryLoop& loop : loops)
        {
            assert(loop.closed);
            assert(loop.vertexCount() == kSegments);
            assert(loop.edgeCount() == kSegments);
        }

        assert(signedDoubleArea(mesh, outerLoop->vertices) > 0.0);
        assert(signedDoubleArea(mesh, innerLoop->vertices) < 0.0);
    }

} // namespace

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " [output-file.vtk]\n";
        return 1;
    }

    const std::string outputPath = argc == 2
        ? argv[1]
        : "annular_disk.vtk";

    const TriangleMesh annularDisk = makeAnnularDisk();
    verifyAnnularDisk(annularDisk);
    writeTriangleMeshToVtk(annularDisk, outputPath);

    std::cout << "Annular disk exported to " << outputPath << "\n";
    std::cout << "Open it in ParaView and select Surface With Edges.\n";
    return 0;
}
