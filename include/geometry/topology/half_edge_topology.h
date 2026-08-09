#pragma once

#include "geometry/mesh_ids.h"

#include <cstddef>
#include <vector>

namespace dsa::geometry
{

    class TriangleMesh;
    class TriangleTopology;

    class HalfEdgeTopology
    {
    public:
        HalfEdgeTopology() = default;

        std::size_t halfEdgeCount() const noexcept;

        TriangleId triangle(HalfEdgeId halfEdge) const;
        std::size_t localEdge(HalfEdgeId halfEdge) const;

        EdgeId edge(HalfEdgeId halfEdge) const;

        HalfEdgeId next(HalfEdgeId halfEdge) const;
        HalfEdgeId previous(HalfEdgeId halfEdge) const;
        HalfEdgeId twin(HalfEdgeId halfEdge) const;

        bool hasTwin(HalfEdgeId halfEdge) const;
        bool isBoundary(HalfEdgeId halfEdge) const;

        HalfEdgeId nextBoundary(HalfEdgeId boundaryHalfEdge) const;

        VertexId origin(
            const TriangleMesh& mesh,
            HalfEdgeId halfEdge
        ) const;

        VertexId destination(
            const TriangleMesh& mesh,
            HalfEdgeId halfEdge
        ) const;

    private:
        friend HalfEdgeTopology buildHalfEdgeTopology(
            const TriangleMesh& mesh,
            const TriangleTopology& topology
        );

        std::vector<EdgeId> edges_;
        std::vector<HalfEdgeId> twins_;
    };

    HalfEdgeTopology buildHalfEdgeTopology(
        const TriangleMesh& mesh,
        const TriangleTopology& topology
    );

} // namespace dsa::geometry