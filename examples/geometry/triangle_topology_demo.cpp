#include "geometry/primitives/triangle_mesh.h"
#include "geometry/topology/boundary.h"
#include "geometry/topology/edge.h"
#include "geometry/topology/half_edge_topology.h"
#include "geometry/topology/orientation.h"
#include "geometry/topology/triangle_topology.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dsa::geometry;

namespace
{

    EdgeId findEdgeId(const TriangleTopology& topology, const Edge& edge)
    {
        for (EdgeId edgeId = 0; edgeId < topology.edgeCount(); ++edgeId)
            if (topology.edge(edgeId) == edge)
                return edgeId;

        throw std::logic_error("Expected edge is absent from topology.");
    }

    bool contains(const TriangleTopology::TriangleNeighbors& neighbors, TriangleId triangleId)
    {
        return std::find(neighbors.begin(), neighbors.end(), triangleId) != neighbors.end();
    }

    void verifyEdge()
    {
        const Edge forward{2, 7};
        const Edge reverse{7, 2};
        const Edge other{2, 8};

        assert(forward.first() == 2);
        assert(forward.second() == 7);
        assert(forward == reverse);
        assert(forward != other);
        assert(forward < other);
        assert(forward.contains(2));
        assert(forward.contains(7));
        assert(!forward.contains(3));
        assert(EdgeHash{}(forward) == EdgeHash{}(reverse));

        bool rejectedDegenerateEdge = false;
        try
        {
            [[maybe_unused]] const Edge invalid{4, 4};
        }
        catch (const std::invalid_argument&)
        {
            rejectedDegenerateEdge = true;
        }
        assert(rejectedDegenerateEdge);
    }

    void verifyManifoldTopology()
    {
        TriangleMesh square;
        const VertexId bottomLeft = square.addVertex({0.0, 0.0, 0.0});
        const VertexId bottomRight = square.addVertex({1.0, 0.0, 0.0});
        const VertexId topRight = square.addVertex({1.0, 1.0, 0.0});
        const VertexId topLeft = square.addVertex({0.0, 1.0, 0.0});

        const TriangleId lowerRight = square.addTriangle({bottomLeft, bottomRight, topRight});
        const TriangleId upperLeft = square.addTriangle({bottomLeft, topRight, topLeft});

        const TriangleTopology topology = buildTriangleTopology(square);
        const EdgeId diagonal = findEdgeId(topology, Edge{bottomLeft, topRight});

        assert(topology.vertexCount() == 4);
        assert(topology.triangleCount() == 2);
        assert(topology.edgeCount() == 5);
        assert(topology.isManifold());
        assert(topology.isInteriorEdge(diagonal));
        assert(!topology.isBoundaryEdge(diagonal));
        assert(!topology.isNonManifoldEdge(diagonal));

        const EdgeIncidence& diagonalIncidence = topology.edgeIncidence(diagonal);
        assert((diagonalIncidence.edge == Edge{bottomLeft, topRight}));
        assert((diagonalIncidence.triangles == std::vector<TriangleId>{lowerRight, upperLeft}));

        const auto& lowerRightEdges = topology.triangleEdges(lowerRight);
        const auto& upperLeftEdges = topology.triangleEdges(upperLeft);
        assert(std::count(lowerRightEdges.begin(), lowerRightEdges.end(), diagonal) == 1);
        assert(std::count(upperLeftEdges.begin(), upperLeftEdges.end(), diagonal) == 1);
        assert(contains(topology.triangleNeighbors(lowerRight), upperLeft));
        assert(contains(topology.triangleNeighbors(upperLeft), lowerRight));

        for (EdgeId edgeId = 0; edgeId < topology.edgeCount(); ++edgeId)
        {
            if (edgeId != diagonal)
            {
                assert(topology.isBoundaryEdge(edgeId));
                assert(!topology.isInteriorEdge(edgeId));
                assert(!topology.isNonManifoldEdge(edgeId));
            }
        }

        assert((topology.incidentTriangles(bottomLeft) == std::vector<TriangleId>{lowerRight, upperLeft}));
        assert(topology.incidentTriangles(bottomRight) == std::vector<TriangleId>{lowerRight});
        assert((topology.incidentTriangles(topRight) == std::vector<TriangleId>{lowerRight, upperLeft}));
        assert(topology.incidentTriangles(topLeft) == std::vector<TriangleId>{upperLeft});

        const HalfEdgeTopology halfEdges = buildHalfEdgeTopology(square, topology);

        const HalfEdgeId lowerRightDiagonal = 3 * lowerRight + 2;
        const HalfEdgeId upperLeftDiagonal = 3 * upperLeft;

        assert(halfEdges.halfEdgeCount() == 6);
        assert(halfEdges.triangle(lowerRightDiagonal) == lowerRight);
        assert(halfEdges.localEdge(lowerRightDiagonal) == 2);
        assert(halfEdges.edge(lowerRightDiagonal) == diagonal);
        assert(halfEdges.next(lowerRightDiagonal) == 3 * lowerRight);
        assert(halfEdges.previous(lowerRightDiagonal) == 3 * lowerRight + 1);
        assert(halfEdges.twin(lowerRightDiagonal) == upperLeftDiagonal);
        assert(halfEdges.twin(upperLeftDiagonal) == lowerRightDiagonal);
        assert(halfEdges.hasTwin(lowerRightDiagonal));
        assert(!halfEdges.isBoundary(lowerRightDiagonal));
        assert(halfEdges.origin(square, lowerRightDiagonal) == topRight);
        assert(halfEdges.destination(square, lowerRightDiagonal) == bottomLeft);

        const HalfEdgeId firstBoundaryHalfEdge = 3 * lowerRight;
        assert(halfEdges.isBoundary(firstBoundaryHalfEdge));
        assert(halfEdges.nextBoundary(firstBoundaryHalfEdge) == 3 * lowerRight + 1);

        const std::vector<BoundaryLoop> loops = extractBoundaryLoops(square, halfEdges);

        assert(loops.size() == 1);
        assert(loops.front().closed);
        assert((loops.front().vertices ==
                std::vector<VertexId>{bottomLeft, bottomRight, topRight, topLeft}));
        assert((loops.front().edges ==
                std::vector<EdgeId>{
                    findEdgeId(topology, Edge{bottomLeft, bottomRight}),
                    findEdgeId(topology, Edge{bottomRight, topRight}),
                    findEdgeId(topology, Edge{topRight, topLeft}),
                    findEdgeId(topology, Edge{topLeft, bottomLeft})
                }));
    }

    void verifyNonManifoldTopology()
    {
        TriangleMesh mesh;
        const VertexId first = mesh.addVertex({0.0, 0.0, 0.0});
        const VertexId second = mesh.addVertex({1.0, 0.0, 0.0});
        const VertexId third = mesh.addVertex({0.0, 1.0, 0.0});
        const VertexId fourth = mesh.addVertex({0.0, -1.0, 0.0});
        const VertexId fifth = mesh.addVertex({0.0, 0.0, 1.0});

        const TriangleId firstTriangle = mesh.addTriangle({first, second, third});
        const TriangleId secondTriangle = mesh.addTriangle({second, first, fourth});
        const TriangleId thirdTriangle = mesh.addTriangle({first, second, fifth});

        const TriangleTopology topology = buildTriangleTopology(mesh);
        const EdgeId sharedEdge = findEdgeId(topology, Edge{first, second});

        assert(!topology.isManifold());
        assert(topology.isNonManifoldEdge(sharedEdge));
        assert(!topology.isBoundaryEdge(sharedEdge));
        assert(!topology.isInteriorEdge(sharedEdge));
        assert(topology.nonManifoldEdges() == std::vector<EdgeId>{sharedEdge});
        assert((topology.edgeIncidence(sharedEdge).triangles ==
                std::vector<TriangleId>{firstTriangle, secondTriangle, thirdTriangle}));

        assert(topology.triangleNeighbors(firstTriangle)[0] == InvalidTriangleId);
        assert(topology.triangleNeighbors(secondTriangle)[0] == InvalidTriangleId);
        assert(topology.triangleNeighbors(thirdTriangle)[0] == InvalidTriangleId);

        const OrientationAnalysis orientation = analyzeOrientation(mesh, topology);
        assert(!orientation.orientable);
        assert(!orientation.consistentlyOriented);
        assert(orientation.connectedComponents == 1);
        assert(!isConsistentlyOriented(mesh, topology));

        bool rejectedNonManifoldHalfEdgeTopology = false;
        try
        {
            [[maybe_unused]] const HalfEdgeTopology halfEdges =
                buildHalfEdgeTopology(mesh, topology);
        }
        catch (const std::invalid_argument&)
        {
            rejectedNonManifoldHalfEdgeTopology = true;
        }
        assert(rejectedNonManifoldHalfEdgeTopology);
    }

    void verifyOrientationCorrection()
    {
        const TriangleMesh square{
            {
                Point3{0.0, 0.0, 0.0}, Point3{1.0, 0.0, 0.0},
                Point3{1.0, 1.0, 0.0}, Point3{0.0, 1.0, 0.0}
            },
            {
                TriangleIndices{0, 1, 2},
                TriangleIndices{2, 0, 3}
            }
        };

        const TriangleTopology topology = buildTriangleTopology(square);
        const EdgeId diagonal = findEdgeId(topology, Edge{0, 2});

        assert(!areAdjacentTrianglesConsistentlyOriented(
            square,
            0,
            1,
            diagonal,
            topology
        ));
        assert(!isConsistentlyOriented(square, topology));

        const OrientationAnalysis analysis = analyzeOrientation(square, topology);

        assert(analysis.orientable);
        assert(!analysis.consistentlyOriented);
        assert(analysis.connectedComponents == 1);
        assert((analysis.flipTriangles == std::vector<std::uint8_t>{0, 1}));

        bool rejectedInconsistentOrientation = false;
        try
        {
            [[maybe_unused]] const HalfEdgeTopology halfEdges =
                buildHalfEdgeTopology(square, topology);
        }
        catch (const std::invalid_argument&)
        {
            rejectedInconsistentOrientation = true;
        }
        assert(rejectedInconsistentOrientation);

        TriangleMesh orientedSquare = square;
        applyOrientation(orientedSquare, analysis);

        const TriangleTopology orientedTopology =
            buildTriangleTopology(orientedSquare);

        assert(isConsistentlyOriented(orientedSquare, orientedTopology));
        [[maybe_unused]] const HalfEdgeTopology halfEdges =
            buildHalfEdgeTopology(orientedSquare, orientedTopology);
    }

    void verifyIcosahedronTopology()
    {
        constexpr double goldenRatio = 1.6180339887498948482;

        const TriangleMesh icosahedron{
            {
                Point3{-1.0, goldenRatio, 0.0}, Point3{1.0, goldenRatio, 0.0},
                Point3{-1.0, -goldenRatio, 0.0}, Point3{1.0, -goldenRatio, 0.0},
                Point3{0.0, -1.0, goldenRatio}, Point3{0.0, 1.0, goldenRatio},
                Point3{0.0, -1.0, -goldenRatio}, Point3{0.0, 1.0, -goldenRatio},
                Point3{goldenRatio, 0.0, -1.0}, Point3{goldenRatio, 0.0, 1.0},
                Point3{-goldenRatio, 0.0, -1.0}, Point3{-goldenRatio, 0.0, 1.0}
            },
            {
                TriangleIndices{0, 11, 5}, TriangleIndices{0, 5, 1},
                TriangleIndices{0, 1, 7}, TriangleIndices{0, 7, 10},
                TriangleIndices{0, 10, 11}, TriangleIndices{1, 5, 9},
                TriangleIndices{5, 11, 4}, TriangleIndices{11, 10, 2},
                TriangleIndices{10, 7, 6}, TriangleIndices{7, 1, 8},
                TriangleIndices{3, 9, 4}, TriangleIndices{3, 4, 2},
                TriangleIndices{3, 2, 6}, TriangleIndices{3, 6, 8},
                TriangleIndices{3, 8, 9}, TriangleIndices{4, 9, 5},
                TriangleIndices{2, 4, 11}, TriangleIndices{6, 2, 10},
                TriangleIndices{8, 6, 7}, TriangleIndices{9, 8, 1}
            }
        };

        assert(icosahedron.isValid());
        assert(icosahedron.isGeometricallyValid());

        const TriangleTopology topology = buildTriangleTopology(icosahedron);

        assert(topology.vertexCount() == 12);
        assert(topology.triangleCount() == 20);
        assert(topology.edgeCount() == 30);
        assert(topology.vertexCount() - topology.edgeCount() + topology.triangleCount() == 2);
        assert(topology.isManifold());
        assert(topology.nonManifoldEdges().empty());

        const HalfEdgeTopology halfEdges = buildHalfEdgeTopology(icosahedron, topology);
        assert(halfEdges.halfEdgeCount() == 60);

        for (HalfEdgeId halfEdge = 0; halfEdge < halfEdges.halfEdgeCount(); ++halfEdge)
        {
            assert(halfEdges.hasTwin(halfEdge));
            assert(!halfEdges.isBoundary(halfEdge));
            assert(halfEdges.twin(halfEdges.twin(halfEdge)) == halfEdge);
            assert(halfEdges.destination(icosahedron, halfEdge) ==
                   halfEdges.origin(icosahedron, halfEdges.twin(halfEdge)));
        }

        assert(extractBoundaryLoops(icosahedron, halfEdges).empty());

        for (VertexId vertexId = 0; vertexId < topology.vertexCount(); ++vertexId)
            assert(topology.incidentTriangles(vertexId).size() == 5);

        for (EdgeId edgeId = 0; edgeId < topology.edgeCount(); ++edgeId)
        {
            const EdgeIncidence& incidence = topology.edgeIncidence(edgeId);
            assert(topology.isInteriorEdge(edgeId));
            assert(!topology.isBoundaryEdge(edgeId));
            assert(!topology.isNonManifoldEdge(edgeId));
            assert(incidence.triangles.size() == 2);
        }

        for (TriangleId triangleId = 0; triangleId < topology.triangleCount(); ++triangleId)
        {
            const auto& edges = topology.triangleEdges(triangleId);
            const auto& neighbors = topology.triangleNeighbors(triangleId);

            for (std::size_t localEdge = 0; localEdge < edges.size(); ++localEdge)
            {
                const TriangleId neighbor = neighbors[localEdge];
                assert(neighbor != InvalidTriangleId);
                assert(std::count(neighbors.begin(), neighbors.end(), neighbor) == 1);

                const auto& neighborEdges = topology.triangleEdges(neighbor);
                assert(std::count(neighborEdges.begin(), neighborEdges.end(), edges[localEdge]) == 1);
            }
        }
    }

    void verifyInvalidMeshIsRejected()
    {
        const TriangleMesh invalidMesh{
            {Point3{0.0, 0.0, 0.0}},
            {TriangleIndices{0, 1, 2}}
        };

        bool rejectedInvalidMesh = false;
        try
        {
            [[maybe_unused]] const TriangleTopology topology = buildTriangleTopology(invalidMesh);
        }
        catch (const std::invalid_argument&)
        {
            rejectedInvalidMesh = true;
        }
        assert(rejectedInvalidMesh);
    }

} // namespace

int main()
{
    std::cout << "=== Triangle topology checks ===\n";

    verifyEdge();
    verifyManifoldTopology();
    verifyNonManifoldTopology();
    verifyOrientationCorrection();
    verifyIcosahedronTopology();
    verifyInvalidMeshIsRejected();

    std::cout << "All edge and triangle-topology checks passed.\n";
    return 0;
}
