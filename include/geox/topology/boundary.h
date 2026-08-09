#pragma once

#include "geox/mesh_ids.h"

#include <cstddef>
#include <vector>

namespace geox
{

    class TriangleMesh;
    class TriangleTopology;
    class HalfEdgeTopology;

    struct BoundaryLoop
    {
        std::vector<VertexId> vertices;
        std::vector<EdgeId> edges;

        bool closed = false;

        std::size_t vertexCount() const noexcept;
        std::size_t edgeCount() const noexcept;
    };

    std::vector<VertexId> boundaryVertices(
        const TriangleTopology& topology
    );

    std::vector<EdgeId> boundaryEdges(
        const TriangleTopology& topology
    );

    std::vector<BoundaryLoop> extractBoundaryLoops(
        const TriangleMesh& mesh,
        const HalfEdgeTopology& topology
    );

} // namespace geox
