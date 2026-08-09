#include "geometry/topology/triangle_topology.h"

namespace dsa::geometry
{

    // MARK: EdgeIncidence
    bool EdgeIncidence::isBoundary() const noexcept
    {
        return triangles.size() == 1;
    }

    bool EdgeIncidence::isInteriorManifold() const noexcept
    {
        return triangles.size() == 2;
    }

    bool EdgeIncidence::isNonManifold() const noexcept
    {
        return triangles.size() > 2;
    }

    // MARK: TriangleTopology
    using TriangleEdges = TriangleTopology::TriangleEdges;
    using TriangleNeighbors = TriangleTopology::TriangleNeighbors;

    std::size_t TriangleTopology::vertexCount() const noexcept { return vertexCount_; }
    std::size_t TriangleTopology::triangleCount() const noexcept { return triangleCount_; }
    std::size_t TriangleTopology::edgeCount() const noexcept { return edges_.size(); }    

    const Edge& TriangleTopology::edge(EdgeId edgeId) const
    {
        return edges_.at(edgeId).edge;
    }

    const EdgeIncidence& TriangleTopology::edgeIncidence(EdgeId edgeId) const
    {
        return edges_.at(edgeId);
    }

    const TriangleEdges& TriangleTopology::triangleEdges(TriangleId triangleId) const
    { 
        return triangleEdges_.at(triangleId);
    }

    const TriangleNeighbors& TriangleTopology::triangleNeighbors(TriangleId triangleId) const
    {
        return triangleNeighbors_.at(triangleId);
    }

    const std::vector<TriangleId>& TriangleTopology::incidentTriangles(VertexId vertexId) const
    {
        return vertexTriangles_.at(vertexId);
    }

    bool TriangleTopology::isBoundaryEdge(EdgeId edgeId) const
    {
        return edges_.at(edgeId).isBoundary();
    }

    bool TriangleTopology::isInteriorEdge(EdgeId edgeId) const
    {
        return edges_.at(edgeId).isInteriorManifold();
    }

    bool TriangleTopology::isNonManifoldEdge(EdgeId edgeId) const
    {
        return edges_.at(edgeId).isNonManifold();
    }

    bool TriangleTopology::isManifold() const noexcept { return manifold_; }

    std::vector<EdgeId> TriangleTopology::nonManifoldEdges() const
    {
        std::vector<EdgeId> nonManifoldEdges;

        for (EdgeId edgeId = 0; edgeId < edges_.size(); ++edgeId)
            if (edges_.at(edgeId).isNonManifold()) nonManifoldEdges.push_back(edgeId);

        return nonManifoldEdges;
    }

    // MARK: BuildTriangleTopology
    TriangleTopology buildTriangleTopology(const TriangleMesh& mesh)
    {
        TriangleTopology topology;

        return topology;
    }

} // namespace dsa::geometry