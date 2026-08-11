#include "test_framework.h"

#include "geox/geox.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace
{
    geox::dim3::EdgeId findEdgeId(const geox::dim3::TriangleTopology& topology, const geox::dim3::Edge& edge)
    {
        for (geox::dim3::EdgeId edgeId = 0; edgeId < topology.edgeCount(); ++edgeId)
        {
            if (topology.edge(edgeId) == edge)
                return edgeId;
        }

        throw std::runtime_error("Expected edge is absent from topology.");
    }

    bool contains(
        const geox::dim3::TriangleTopology::TriangleNeighbors& neighbors,
        geox::dim3::TriangleId triangleId
    )
    {
        return std::find(neighbors.begin(), neighbors.end(), triangleId) != neighbors.end();
    }

    geox::dim3::TriangleMesh makeSquare()
    {
        return {
            {
                geox::dim3::Point{0.0, 0.0, 0.0}, geox::dim3::Point{1.0, 0.0, 0.0},
                geox::dim3::Point{1.0, 1.0, 0.0}, geox::dim3::Point{0.0, 1.0, 0.0}
            },
            {
                geox::dim3::TriangleIndices{0, 1, 2}, geox::dim3::TriangleIndices{0, 2, 3}
            }
        };
    }
} // namespace

GEOX_TEST(edge_is_undirected_orderable_and_rejects_degenerate_input)
{
    using namespace geox;

    const dim3::Edge forward{2, 7};
    const dim3::Edge reverse{7, 2};
    GEOX_REQUIRE(forward.first() == 2);
    GEOX_REQUIRE(forward.second() == 7);
    GEOX_REQUIRE(forward == reverse);
    GEOX_REQUIRE(forward < dim3::Edge{2, 8});
    GEOX_REQUIRE(forward.contains(2));
    GEOX_REQUIRE(!forward.contains(3));
    GEOX_REQUIRE(dim3::EdgeHash{}(forward) == dim3::EdgeHash{}(reverse));
    GEOX_REQUIRE_THROWS_AS((dim3::Edge{4, 4}), std::invalid_argument);
}

GEOX_TEST(triangle_topology_reports_adjacency_and_boundary_for_a_square)
{
    using namespace geox;

    const dim3::TriangleMesh square = makeSquare();
    const dim3::TriangleTopology topology = buildTriangleTopology(square);
    const dim3::EdgeId diagonal = findEdgeId(topology, dim3::Edge{0, 2});

    GEOX_REQUIRE(topology.vertexCount() == 4);
    GEOX_REQUIRE(topology.triangleCount() == 2);
    GEOX_REQUIRE(topology.edgeCount() == 5);
    GEOX_REQUIRE(topology.isManifold());
    GEOX_REQUIRE(topology.isInteriorEdge(diagonal));
    GEOX_REQUIRE(topology.edgeIncidence(diagonal).isInteriorManifold());
    GEOX_REQUIRE(contains(topology.triangleNeighbors(0), 1));
    GEOX_REQUIRE(contains(topology.triangleNeighbors(1), 0));
    GEOX_REQUIRE(topology.incidentTriangles(0) == std::vector<dim3::TriangleId>{0, 1});
    GEOX_REQUIRE(topology.nonManifoldEdges().empty());

    const std::vector<dim3::EdgeId> edges = boundaryEdges(topology);
    GEOX_REQUIRE(edges.size() == 4);
    GEOX_REQUIRE(boundaryVertices(topology) == std::vector<dim3::VertexId>{0, 1, 2, 3});
    GEOX_REQUIRE(!isClosed(topology));
}

GEOX_TEST(half_edge_topology_and_boundary_loops_follow_oriented_square_boundary)
{
    using namespace geox;

    const dim3::TriangleMesh square = makeSquare();
    const dim3::TriangleTopology topology = buildTriangleTopology(square);
    const dim3::EdgeId diagonal = findEdgeId(topology, dim3::Edge{0, 2});
    const dim3::HalfEdgeTopology halfEdges = buildHalfEdgeTopology(square, topology);

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

    const std::vector<dim3::BoundaryLoop> loops = extractBoundaryLoops(square, halfEdges);
    GEOX_REQUIRE(loops.size() == 1);
    GEOX_REQUIRE(loops.front().closed);
    GEOX_REQUIRE(loops.front().vertexCount() == 4);
    GEOX_REQUIRE(loops.front().edgeCount() == 4);
    GEOX_REQUIRE(loops.front().vertices == std::vector<dim3::VertexId>{0, 1, 2, 3});
}

GEOX_TEST(orientation_analysis_corrects_inconsistent_winding)
{
    using namespace geox;

    dim3::TriangleMesh square = makeSquare();
    square.reverseTriangleOrientation(1);
    dim3::TriangleTopology topology = buildTriangleTopology(square);
    const dim3::EdgeId diagonal = findEdgeId(topology, dim3::Edge{0, 2});

    GEOX_REQUIRE(!areAdjacentTrianglesConsistentlyOriented(square, 0, 1, diagonal, topology));
    const dim3::OrientationAnalysis analysis = analyzeOrientation(square, topology);
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

    const dim3::TriangleMesh mesh{
        {
            dim3::Point{}, dim3::Point{1.0, 0.0, 0.0}, dim3::Point{0.0, 1.0, 0.0},
            dim3::Point{0.0, -1.0, 0.0}, dim3::Point{0.0, 0.0, 1.0}
        },
        {
            dim3::TriangleIndices{0, 1, 2}, dim3::TriangleIndices{1, 0, 3}, dim3::TriangleIndices{0, 1, 4}
        }
    };
    const dim3::TriangleTopology topology = buildTriangleTopology(mesh);
    const dim3::EdgeId shared = findEdgeId(topology, dim3::Edge{0, 1});

    GEOX_REQUIRE(!topology.isManifold());
    GEOX_REQUIRE(topology.isNonManifoldEdge(shared));
    GEOX_REQUIRE(topology.nonManifoldEdges() == std::vector<dim3::EdgeId>{shared});
    GEOX_REQUIRE(!analyzeOrientation(mesh, topology).orientable);
    GEOX_REQUIRE(!isClosed(topology));
    GEOX_REQUIRE_THROWS_AS(buildHalfEdgeTopology(mesh, topology), std::invalid_argument);
}

GEOX_TEST(closed_tetrahedron_has_oriented_volume_and_reverse_changes_its_sign)
{
    using namespace geox;

    dim3::TriangleMesh tetrahedron{
        {
            dim3::Point{}, dim3::Point{1.0, 0.0, 0.0}, dim3::Point{0.0, 1.0, 0.0}, dim3::Point{0.0, 0.0, 1.0}
        },
        {
            dim3::TriangleIndices{0, 2, 1}, dim3::TriangleIndices{0, 1, 3},
            dim3::TriangleIndices{0, 3, 2}, dim3::TriangleIndices{1, 2, 3}
        }
    };
    const dim3::TriangleTopology topology = buildTriangleTopology(tetrahedron);
    const double volume = signedVolume(tetrahedron);

    GEOX_REQUIRE(topology.isManifold());
    GEOX_REQUIRE(isClosed(topology));
    GEOX_REQUIRE(isConsistentlyOriented(tetrahedron, topology));
    GEOX_REQUIRE_CLOSE(volume, 1.0 / 6.0);
    reverseOrientation(tetrahedron);
    GEOX_REQUIRE_CLOSE(signedVolume(tetrahedron), -volume);
}
