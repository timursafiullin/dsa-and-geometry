#include "geox/topology/boundary.h"

#include "geox/primitives/triangle_mesh.h"
#include "geox/topology/half_edge_topology.h"
#include "geox/topology/triangle_topology.h"

#include <cstdint>
#include <utility>

namespace geox
{

    std::size_t BoundaryLoop::vertexCount() const noexcept { return vertices.size(); }
    std::size_t BoundaryLoop::edgeCount() const noexcept { return edges.size(); }

    std::vector<VertexId> boundaryVertices(
        const TriangleTopology& topology
    )
    {
        std::vector<bool> isBoundaryVertex(topology.vertexCount(), false);

        for (EdgeId edgeId : boundaryEdges(topology))
        {
            const Edge& edge = topology.edge(edgeId);
            isBoundaryVertex[edge.first()] = true;
            isBoundaryVertex[edge.second()] = true;
        }

        std::vector<VertexId> result;

        for (VertexId vertexId = 0; vertexId < topology.vertexCount(); ++vertexId)
            if (isBoundaryVertex[vertexId])
                result.push_back(vertexId);

        return result;
    }

    std::vector<EdgeId> boundaryEdges(
        const TriangleTopology& topology
    )
    {
        std::vector<EdgeId> result;

        for (EdgeId edgeId = 0; edgeId < topology.edgeCount(); ++edgeId)
            if (topology.isBoundaryEdge(edgeId))
                result.push_back(edgeId);

        return result;
    }

    std::vector<BoundaryLoop> extractBoundaryLoops(
        const TriangleMesh& mesh,
        const HalfEdgeTopology& topology
    )
    {
        std::vector<BoundaryLoop> result;

        std::vector<std::uint8_t> used(topology.halfEdgeCount(), 0);

        for (HalfEdgeId firstHalfEdge = 0;
             firstHalfEdge < topology.halfEdgeCount();
             ++firstHalfEdge)
        {
            if (!topology.isBoundary(firstHalfEdge) || used[firstHalfEdge])
                continue;

            BoundaryLoop loop;
            HalfEdgeId currentHalfEdge = firstHalfEdge;

            do
            {
                used[currentHalfEdge] = 1;

                loop.vertices.push_back(
                    topology.origin(mesh, currentHalfEdge)
                );

                loop.edges.push_back(
                    topology.edge(currentHalfEdge)
                );

                currentHalfEdge = topology.nextBoundary(currentHalfEdge);
            }
            while (currentHalfEdge != firstHalfEdge);

            loop.closed = true;
            result.push_back(std::move(loop));
        }

        return result;
    }

} // namespace geox
