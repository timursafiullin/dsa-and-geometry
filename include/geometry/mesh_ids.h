#pragma once

#include <cstdint>
#include <limits>

namespace dsa::geometry
{

    using VertexId = std::uint32_t;
    using TriangleId = std::uint32_t;
    using EdgeId = std::uint32_t;
    using HalfEdgeId = std::uint32_t;

    inline constexpr TriangleId InvalidTriangleId =
        std::numeric_limits<TriangleId>::max();

    inline constexpr EdgeId InvalidEdgeId =
        std::numeric_limits<EdgeId>::max();

    inline constexpr HalfEdgeId InvalidHalfEdgeId =
        std::numeric_limits<HalfEdgeId>::max();

}