#include "geometry/topology/edge.h"

#include <algorithm>
#include <functional>
#include <cstdint>
#include <stdexcept>

namespace dsa::geometry
{

    // MARK: Edge
    Edge::Edge(VertexId firstVertex, VertexId secondVertex)
        : first_(std::min(firstVertex, secondVertex))
        , second_(std::max(firstVertex, secondVertex))
    {
        if (firstVertex == secondVertex)
            throw std::invalid_argument("An edge must connect two distinct vertices.");
    }
    
    VertexId Edge::first() const noexcept { return first_; }

    VertexId Edge::second() const noexcept { return second_; }

    bool Edge::contains(VertexId vertex) const noexcept
    {
        return vertex == first_ || vertex == second_;
    }

    bool Edge::operator==(const Edge& other) const noexcept
    {
        return first_ == other.first() && second_ == other.second();
    }

    bool Edge::operator!=(const Edge& other) const noexcept
    {
        return !(*this == other);
    }

    bool Edge::operator<(const Edge& other) const noexcept
    {
        return  (first_ < other.first()) ||
                (first_ == other.first() && second_ < other.second());
    }


    // MARK: EdgeHash
    std::size_t EdgeHash::operator()(const Edge& edge) const noexcept
    {
        const std::uint64_t key =
            (static_cast<std::uint64_t>(edge.first()) << 32) |
            static_cast<std::uint64_t>(edge.second());

        return std::hash<std::uint64_t>{}(key);
    }

} // namespace dsa::geometry