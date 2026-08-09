#pragma once

#include "geometry/mesh_ids.h"
#include "geometry/topology/edge.h"

#include <array>
#include <cstddef>
#include <vector>

namespace dsa::geometry
{

    class TriangleMesh;

    struct EdgeIncidence
    {
        Edge edge;
        std::vector<TriangleId> triangles;

        bool isBoundary() const noexcept;
        bool isInteriorManifold() const noexcept;
        bool isNonManifold() const noexcept;
    };

    class TriangleTopology
    {
    public:
        // For triangle vertices [v0, v1, v2], local edges are:
        // 0: (v0, v1), 1: (v1, v2), 2: (v2, v0).
        // triangleEdges_[triangleId][i] and triangleNeighbors_[triangleId][i]
        // describe the same local edge i.
        using TriangleEdges = std::array<EdgeId, 3>;
        using TriangleNeighbors = std::array<TriangleId, 3>;

        TriangleTopology() = default;

        std::size_t vertexCount() const noexcept;
        std::size_t triangleCount() const noexcept;
        std::size_t edgeCount() const noexcept;

        const Edge& edge(EdgeId edgeId) const;
        const EdgeIncidence& edgeIncidence(EdgeId edgeId) const;

        const TriangleEdges& triangleEdges(TriangleId triangleId) const;

        const TriangleNeighbors& triangleNeighbors(TriangleId triangleId) const;

        const std::vector<TriangleId>& incidentTriangles(VertexId vertexId) const;

        bool isBoundaryEdge(EdgeId edgeId) const;
        bool isInteriorEdge(EdgeId edgeId) const;
        bool isNonManifoldEdge(EdgeId edgeId) const;

        bool isManifold() const noexcept;
        std::vector<EdgeId> nonManifoldEdges() const;

    private:
        friend TriangleTopology buildTriangleTopology(
            const TriangleMesh& mesh
        );

        std::size_t vertexCount_ = 0;
        std::size_t triangleCount_ = 0;

        std::vector<EdgeIncidence> edges_;

        std::vector<TriangleEdges> triangleEdges_;
        std::vector<TriangleNeighbors> triangleNeighbors_;

        std::vector<std::vector<TriangleId>> vertexTriangles_;

        bool manifold_ = true;
    };

    TriangleTopology buildTriangleTopology(
        const TriangleMesh& mesh
    );

} // namespace dsa::geometry