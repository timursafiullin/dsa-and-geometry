#pragma once

#include "geox/dim3/mesh/ids.h"

#include <cstddef>

namespace geox::dim3
{

    class Edge
    {
    public:
        Edge(VertexId firstVertex, VertexId secondVertex);

        VertexId first() const noexcept;
        VertexId second() const noexcept;

        bool contains(VertexId vertex) const noexcept;

        bool operator==(const Edge& other) const noexcept;
        bool operator!=(const Edge& other) const noexcept;
        bool operator<(const Edge& other) const noexcept;

    private:
        VertexId first_;
        VertexId second_;
    };

    struct EdgeHash
    {
        std::size_t operator()(const Edge& edge) const noexcept;
    };

} // namespace geox::dim3