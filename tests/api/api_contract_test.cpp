#include "test_framework.h"

#include "geox/geox.h"

#include <type_traits>
#include <variant>

namespace
{
    template <typename Point>
    auto adlDistance(const Point& first, const Point& second)
    {
        return distance(first, second);
    }
}

static_assert(!std::is_same_v<geox::dim2::Point, geox::dim3::Point>);
static_assert(!std::is_convertible_v<geox::dim2::Point, geox::dim3::Point>);
static_assert(!std::is_convertible_v<geox::dim3::Point, geox::dim2::Point>);
static_assert(std::is_same_v<
    decltype(&geox::dim3::buildTriangleTopology),
    geox::dim3::TriangleTopology (*)(const geox::dim3::TriangleMesh&)
>);
static_assert(std::is_same_v<
    std::variant_alternative_t<1, geox::dim2::SegmentIntersection>,
    geox::dim2::Point
>);
static_assert(std::is_same_v<
    std::variant_alternative_t<2, geox::dim3::SegmentIntersection>,
    geox::dim3::Segment
>);

GEOX_TEST(dimension_namespaces_preserve_adl)
{
    const geox::dim2::Point point2{0.0, 0.0};
    const geox::dim3::Point point3{0.0, 0.0, 0.0};

    GEOX_REQUIRE_CLOSE(adlDistance(point2, geox::dim2::Point{3.0, 4.0}), 5.0);
    GEOX_REQUIRE_CLOSE(adlDistance(point3, geox::dim3::Point{0.0, 0.0, 2.0}), 2.0);
}
