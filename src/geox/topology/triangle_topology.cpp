#include "geox/topology/triangle_topology.h"
#include "geox/primitives/triangle_mesh.h"

#include <unordered_map>
#include <stdexcept>
#include <array>

namespace
{

    std::size_t findLocalEdge(
        const geox::TriangleTopology::TriangleEdges& triangleEdges,
        geox::EdgeId edgeId
    )
    {
        for (std::size_t localEdge = 0; localEdge < triangleEdges.size(); ++localEdge)
            if (triangleEdges[localEdge] == edgeId)
                return localEdge;

        throw std::logic_error("Triangle does not contain its incident edge.");
    }

} // namespace

namespace geox
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
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot build topology for an invalid triangle mesh.");

        TriangleTopology topology;

        topology.vertexCount_ = mesh.vertexCount();
        topology.triangleCount_ = mesh.triangleCount();

        topology.triangleEdges_.resize(topology.triangleCount_);
        topology.vertexTriangles_.resize(topology.vertexCount_);

        topology.triangleNeighbors_ = std::vector<TriangleNeighbors>(
            topology.triangleCount_,
            {InvalidTriangleId, InvalidTriangleId, InvalidTriangleId}
        );

        std::unordered_map<Edge, EdgeId, EdgeHash> edgeIds;

        for (TriangleId triangleId = 0; triangleId < mesh.triangleCount(); ++triangleId)
        {
            VertexId v0 = mesh.triangle(triangleId).at(0);
            VertexId v1 = mesh.triangle(triangleId).at(1);
            VertexId v2 = mesh.triangle(triangleId).at(2);

            topology.vertexTriangles_.at(v0).push_back(triangleId);
            topology.vertexTriangles_.at(v1).push_back(triangleId);
            topology.vertexTriangles_.at(v2).push_back(triangleId);

            const std::array<Edge, 3> localEdges = {
                Edge(v0, v1),
                Edge(v1, v2),
                Edge(v2, v0)
            };

            for (std::size_t localEdge = 0; localEdge < localEdges.size(); ++localEdge)
            {
                const Edge& edge = localEdges[localEdge];

                const auto [iterator, inserted] = edgeIds.emplace(
                    edge,
                    static_cast<EdgeId>(topology.edges_.size())
                );

                const EdgeId edgeId = iterator->second;

                if (inserted)
                    topology.edges_.push_back({edge, {}});

                topology.triangleEdges_[triangleId][localEdge] = edgeId;
                EdgeIncidence& incidence = topology.edges_.at(edgeId);

                const std::size_t previousTriangleCount = incidence.triangles.size();

                switch (previousTriangleCount)
                {
                    // First time: no neighbors yet
                    case 0:
                        break;

                    // Second time: set neighborhood between triangles
                    case 1:
                    {
                        TriangleId prevTriangleId = incidence.triangles.front();

                        const std::size_t prevLocalEdge = findLocalEdge(
                            topology.triangleEdges_.at(prevTriangleId),
                            edgeId
                        );

                        topology.triangleNeighbors_[triangleId][localEdge] = prevTriangleId;
                        topology.triangleNeighbors_[prevTriangleId][prevLocalEdge] = triangleId;
                        break;
                    }

                    // Third time: edge became non-manifold, taking out the neighborhood
                    case 2:
                    {
                        for (TriangleId prevTriangleId : incidence.triangles)
                        {
                            const std::size_t prevLocalEdge = findLocalEdge(
                                topology.triangleEdges_.at(prevTriangleId),
                                edgeId
                            );

                            topology.triangleNeighbors_[prevTriangleId][prevLocalEdge] = InvalidTriangleId;
                        }
                        
                        topology.manifold_ = false;
                        break;
                    }

                    // Edge is already non-manifold
                    default:
                        break;
                }

                incidence.triangles.push_back(triangleId);
            }
        }

        return topology;
    }

} // namespace geox