#pragma once

#include "geox/dim3/mesh/ids.h"
#include "geox/dim3/mesh/triangle_mesh.h"

#include <cstddef>
#include <vector>

namespace geox::dim3
{

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

} // namespace geox::dim3
