#include "test_framework.h"

#include "geox/primitives/triangle_mesh.h"
#include "geox/topology/boundary.h"
#include "geox/topology/edge.h"
#include "geox/topology/half_edge_topology.h"
#include "geox/topology/orientation.h"
#include "geox/topology/triangle_topology.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace
{
    geox::EdgeId findEdgeId(const geox::TriangleTopology& topology, const geox::Edge& edge)
    {
        for (geox::EdgeId edgeId = 0; edgeId < topology.edgeCount(); ++edgeId)
        {
            if (topology.edge(edgeId) == edge)
                return edgeId;
        }

        throw std::runtime_error("Expected edge is absent from topology.");
    }

    bool contains(
        const geox::TriangleTopology::TriangleNeighbors& neighbors,
        geox::TriangleId triangleId
    )
    {
        return std::find(neighbors.begin(), neighbors.end(), triangleId) != neighbors.end();
    }

    geox::TriangleMesh makeSquare()
    {
        return {
            {
                geox::Point3{0.0, 0.0, 0.0}, geox::Point3{1.0, 0.0, 0.0},
                geox::Point3{1.0, 1.0, 0.0}, geox::Point3{0.0, 1.0, 0.0}
            },
            {
                geox::TriangleIndices{0, 1, 2}, geox::TriangleIndices{0, 2, 3}
            }
        };
    }
} // namespace

GEOX_TEST(edge_is_undirected_orderable_and_rejects_degenerate_input)
{
    using namespace geox;

    const Edge forward{2, 7};
    const Edge reverse{7, 2};
    GEOX_REQUIRE(forward.first() == 2);
    GEOX_REQUIRE(forward.second() == 7);
    GEOX_REQUIRE(forward == reverse);
    GEOX_REQUIRE(forward < Edge{2, 8});
    GEOX_REQUIRE(forward.contains(2));
    GEOX_REQUIRE(!forward.contains(3));
    GEOX_REQUIRE(EdgeHash{}(forward) == EdgeHash{}(reverse));
    GEOX_REQUIRE_THROWS_AS((Edge{4, 4}), std::invalid_argument);
}

GEOX_TEST(triangle_topology_reports_adjacency_and_boundary_for_a_square)
{
    using namespace geox;

    const TriangleMesh square = makeSquare();
    const TriangleTopology topology = buildTriangleTopology(square);
    const EdgeId diagonal = findEdgeId(topology, Edge{0, 2});

    GEOX_REQUIRE(topology.vertexCount() == 4);
    GEOX_REQUIRE(topology.triangleCount() == 2);
    GEOX_REQUIRE(topology.edgeCount() == 5);
    GEOX_REQUIRE(topology.isManifold());
    GEOX_REQUIRE(topology.isInteriorEdge(diagonal));
    GEOX_REQUIRE(topology.edgeIncidence(diagonal).isInteriorManifold());
    GEOX_REQUIRE(contains(topology.triangleNeighbors(0), 1));
    GEOX_REQUIRE(contains(topology.triangleNeighbors(1), 0));
    GEOX_REQUIRE(topology.incidentTriangles(0) == std::vector<TriangleId>{0, 1});
    GEOX_REQUIRE(topology.nonManifoldEdges().empty());

    const std::vector<EdgeId> edges = boundaryEdges(topology);
    GEOX_REQUIRE(edges.size() == 4);
    GEOX_REQUIRE(boundaryVertices(topology) == std::vector<VertexId>{0, 1, 2, 3});
    GEOX_REQUIRE(!isClosed(topology));
}

GEOX_TEST(half_edge_topology_and_boundary_loops_follow_oriented_square_boundary)
{
    using namespace geox;

    const TriangleMesh square = makeSquare();
    const TriangleTopology topology = buildTriangleTopology(square);
    const EdgeId diagonal = findEdgeId(topology, Edge{0, 2});
    const HalfEdgeTopology halfEdges = buildHalfEdgeTopology(square, topology);

    GEOX_REQUIRE(halfEdges.halfEdgeCount() == 6);
    GEOX_REQUIRE(halfEdges.edge(2) == diagonal);
    GEOX_REQUIRE(halfEdges.next(2) == 0);
    GEOX_REQUIRE(halfEdges.previous(2) == 1);
    GEOX_REQUIRE(halfEdges.hasTwin(2));
    GEOX_REQUIRE(halfEdges.twin(2) == 3);
    GEOX_REQUIRE(halfEdges.origin(square, 2) == 2);
    GEOX_REQUIRE(halfEdges.destination(square, 2) == 0);
    GEOX_REQUIRE(halfEdges.isBoundary(0));
    GEOX_REQUIRE(halfEdges.nextBoundary(0) == 1);
    GEOX_REQUIRE_THROWS_AS(halfEdges.nextBoundary(2), std::invalid_argument);
    GEOX_REQUIRE_THROWS_AS(halfEdges.triangle(6), std::out_of_range);

    const std::vector<BoundaryLoop> loops = extractBoundaryLoops(square, halfEdges);
    GEOX_REQUIRE(loops.size() == 1);
    GEOX_REQUIRE(loops.front().closed);
    GEOX_REQUIRE(loops.front().vertexCount() == 4);
    GEOX_REQUIRE(loops.front().edgeCount() == 4);
    GEOX_REQUIRE(loops.front().vertices == std::vector<VertexId>{0, 1, 2, 3});
}

GEOX_TEST(orientation_analysis_corrects_inconsistent_winding)
{
    using namespace geox;

    TriangleMesh square = makeSquare();
    square.reverseTriangleOrientation(1);
    TriangleTopology topology = buildTriangleTopology(square);
    const EdgeId diagonal = findEdgeId(topology, Edge{0, 2});

    GEOX_REQUIRE(!areAdjacentTrianglesConsistentlyOriented(square, 0, 1, diagonal, topology));
    const OrientationAnalysis analysis = analyzeOrientation(square, topology);
    GEOX_REQUIRE(analysis.orientable);
    GEOX_REQUIRE(!analysis.consistentlyOriented);
    GEOX_REQUIRE(analysis.connectedComponents == 1);
    GEOX_REQUIRE(analysis.flipTriangles == std::vector<std::uint8_t>{0, 1});
    GEOX_REQUIRE_THROWS_AS(buildHalfEdgeTopology(square, topology), std::invalid_argument);

    applyOrientation(square, analysis);
    topology = buildTriangleTopology(square);
    GEOX_REQUIRE(isConsistentlyOriented(square, topology));
    GEOX_REQUIRE(buildHalfEdgeTopology(square, topology).halfEdgeCount() == 6);
}

GEOX_TEST(non_manifold_edges_are_detected_and_cannot_build_half_edges)
{
    using namespace geox;

    const TriangleMesh mesh{
        {
            Point3{}, Point3{1.0, 0.0, 0.0}, Point3{0.0, 1.0, 0.0},
            Point3{0.0, -1.0, 0.0}, Point3{0.0, 0.0, 1.0}
        },
        {
            TriangleIndices{0, 1, 2}, TriangleIndices{1, 0, 3}, TriangleIndices{0, 1, 4}
        }
    };
    const TriangleTopology topology = buildTriangleTopology(mesh);
    const EdgeId shared = findEdgeId(topology, Edge{0, 1});

    GEOX_REQUIRE(!topology.isManifold());
    GEOX_REQUIRE(topology.isNonManifoldEdge(shared));
    GEOX_REQUIRE(topology.nonManifoldEdges() == std::vector<EdgeId>{shared});
    GEOX_REQUIRE(!analyzeOrientation(mesh, topology).orientable);
    GEOX_REQUIRE(!isClosed(topology));
    GEOX_REQUIRE_THROWS_AS(buildHalfEdgeTopology(mesh, topology), std::invalid_argument);
}

GEOX_TEST(closed_tetrahedron_has_oriented_volume_and_reverse_changes_its_sign)
{
    using namespace geox;

    TriangleMesh tetrahedron{
        {
            Point3{}, Point3{1.0, 0.0, 0.0}, Point3{0.0, 1.0, 0.0}, Point3{0.0, 0.0, 1.0}
        },
        {
            TriangleIndices{0, 2, 1}, TriangleIndices{0, 1, 3},
            TriangleIndices{0, 3, 2}, TriangleIndices{1, 2, 3}
        }
    };
    const TriangleTopology topology = buildTriangleTopology(tetrahedron);
    const double volume = signedVolume(tetrahedron);

    GEOX_REQUIRE(topology.isManifold());
    GEOX_REQUIRE(isClosed(topology));
    GEOX_REQUIRE(isConsistentlyOriented(tetrahedron, topology));
    GEOX_REQUIRE_CLOSE(volume, 1.0 / 6.0);
    reverseOrientation(tetrahedron);
    GEOX_REQUIRE_CLOSE(signedVolume(tetrahedron), -volume);
}
