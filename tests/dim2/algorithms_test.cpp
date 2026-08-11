#include "test_framework.h"

#include "geox/geox.h"

#include <variant>

namespace
{
    bool samePoint(const geox::dim2::Point& first, const geox::dim2::Point& second)
    {
        return first == second;
    }

    bool samePoint(const geox::dim3::Point& first, const geox::dim3::Point& second)
    {
        return first == second;
    }
} // namespace

GEOX_TEST(segment2_algorithms_handle_projection_membership_and_intersections)
{
    using namespace geox;

    const dim2::Segment horizontal{dim2::Point{0.0, 0.0}, dim2::Point{4.0, 0.0}};
    GEOX_REQUIRE(isPointOnSegment(dim2::Point{2.0, 0.0}, horizontal));
    GEOX_REQUIRE(!isPointOnSegment(dim2::Point{2.0, 0.1}, horizontal));
    GEOX_REQUIRE(samePoint(closestPointOnSegment(dim2::Point{2.0, 3.0}, horizontal), dim2::Point{2.0, 0.0}));
    GEOX_REQUIRE_CLOSE(squaredDistanceToSegment(dim2::Point{2.0, 3.0}, horizontal), 9.0);

    const dim2::SegmentIntersection crossing = intersect(
        dim2::Segment{dim2::Point{0.0, 0.0}, dim2::Point{4.0, 4.0}},
        dim2::Segment{dim2::Point{0.0, 4.0}, dim2::Point{4.0, 0.0}}
    );
    GEOX_REQUIRE(std::holds_alternative<dim2::Point>(crossing));
    GEOX_REQUIRE(samePoint(std::get<dim2::Point>(crossing), dim2::Point{2.0, 2.0}));

    const dim2::SegmentIntersection overlap = intersect(
        horizontal,
        dim2::Segment{dim2::Point{3.0, 0.0}, dim2::Point{1.0, 0.0}}
    );
    GEOX_REQUIRE(std::holds_alternative<dim2::Segment>(overlap));
    GEOX_REQUIRE(samePoint(std::get<dim2::Segment>(overlap).start(), dim2::Point{1.0, 0.0}));
    GEOX_REQUIRE(samePoint(std::get<dim2::Segment>(overlap).end(), dim2::Point{3.0, 0.0}));
    GEOX_REQUIRE(std::holds_alternative<std::monostate>(intersect(
        horizontal,
        dim2::Segment{dim2::Point{0.0, 1.0}, dim2::Point{4.0, 1.0}}
    )));
}

GEOX_TEST(segment3_algorithms_reject_skew_segments_and_handle_collinearity)
{
    using namespace geox;

    const dim3::Segment horizontal{dim3::Point{}, dim3::Point{2.0, 0.0, 0.0}};
    GEOX_REQUIRE(isPointOnSegment(dim3::Point{1.0, 0.0, 0.0}, horizontal));
    GEOX_REQUIRE(!isPointOnSegment(dim3::Point{1.0, 0.0, 0.1}, horizontal));
    GEOX_REQUIRE(samePoint(closestPointOnSegment(dim3::Point{3.0, 1.0, 0.0}, horizontal), dim3::Point{2.0, 0.0, 0.0}));
    GEOX_REQUIRE_CLOSE(squaredDistanceToSegment(dim3::Point{1.0, 2.0, 3.0}, horizontal), 13.0);

    const dim3::SegmentIntersection crossing = intersect(
        horizontal,
        dim3::Segment{dim3::Point{1.0, -1.0, 0.0}, dim3::Point{1.0, 1.0, 0.0}}
    );
    GEOX_REQUIRE(std::holds_alternative<dim3::Point>(crossing));
    GEOX_REQUIRE(samePoint(std::get<dim3::Point>(crossing), dim3::Point{1.0, 0.0, 0.0}));
    GEOX_REQUIRE(std::holds_alternative<std::monostate>(intersect(
        horizontal,
        dim3::Segment{dim3::Point{1.0, -1.0, 1.0}, dim3::Point{1.0, 1.0, 1.0}}
    )));

    const dim3::SegmentIntersection overlap = intersect(
        horizontal,
        dim3::Segment{dim3::Point{1.0, 0.0, 0.0}, dim3::Point{3.0, 0.0, 0.0}}
    );
    GEOX_REQUIRE(std::holds_alternative<dim3::Segment>(overlap));
    GEOX_REQUIRE(samePoint(std::get<dim3::Segment>(overlap).end(), dim3::Point{2.0, 0.0, 0.0}));
}

GEOX_TEST(triangle2_algorithms_calculate_barycentrics_and_handle_degenerate_input)
{
    using namespace geox;

    const dim2::Triangle triangle{dim2::Point{}, dim2::Point{4.0, 0.0}, dim2::Point{0.0, 3.0}};
    const auto barycentric = barycentricCoordinates(dim2::Point{1.0, 1.0}, triangle);
    GEOX_REQUIRE(barycentric.has_value());
    GEOX_REQUIRE_CLOSE(barycentric->l0, 5.0 / 12.0);
    GEOX_REQUIRE_CLOSE(barycentric->l1, 0.25);
    GEOX_REQUIRE_CLOSE(barycentric->l2, 1.0 / 3.0);
    GEOX_REQUIRE(isPointInTriangle(dim2::Point{0.0, 0.0}, triangle));
    GEOX_REQUIRE(!isPointInTriangle(dim2::Point{4.0, 3.0}, triangle));

    const dim2::Triangle flat{dim2::Point{}, dim2::Point{1.0, 1.0}, dim2::Point{2.0, 2.0}};
    GEOX_REQUIRE(!barycentricCoordinates(dim2::Point{1.0, 1.0}, flat).has_value());
    GEOX_REQUIRE(!isPointInTriangle(dim2::Point{1.0, 1.0}, flat));
}

GEOX_TEST(triangle3_algorithms_check_the_plane_and_find_closest_points)
{
    using namespace geox;

    const dim3::Triangle triangle{dim3::Point{}, dim3::Point{1.0, 0.0, 0.0}, dim3::Point{0.0, 1.0, 0.0}};
    const auto barycentric = barycentricCoordinates(dim3::Point{0.25, 0.25, 0.0}, triangle);
    GEOX_REQUIRE(barycentric.has_value());
    GEOX_REQUIRE_CLOSE(barycentric->l0, 0.5);
    GEOX_REQUIRE(isPointInTriangle(dim3::Point{0.25, 0.25, 0.0}, triangle));
    GEOX_REQUIRE(!barycentricCoordinates(dim3::Point{0.25, 0.25, 0.1}, triangle).has_value());
    GEOX_REQUIRE(!isPointInTriangle(dim3::Point{0.25, 0.25, 0.1}, triangle));
    GEOX_REQUIRE(samePoint(
        closestPointOnTriangle(dim3::Point{0.25, 0.25, 2.0}, triangle),
        dim3::Point{0.25, 0.25, 0.0}
    ));
    GEOX_REQUIRE(samePoint(
        closestPointOnEdges(dim3::Point{0.25, 0.25, 2.0}, triangle),
        dim3::Point{0.25, 0.0, 0.0}
    ));
    GEOX_REQUIRE(samePoint(
        closestPointOnTriangle(dim3::Point{0.8, 0.8, 2.0}, triangle),
        dim3::Point{0.5, 0.5, 0.0}
    ));
}
