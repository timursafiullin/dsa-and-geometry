#include "test_framework.h"

#include "geox/algorithms/segment2.h"
#include "geox/algorithms/segment3.h"
#include "geox/algorithms/triangle2.h"
#include "geox/algorithms/triangle3.h"

#include <variant>

namespace
{
    bool samePoint(const geox::Point2& first, const geox::Point2& second)
    {
        return first == second;
    }

    bool samePoint(const geox::Point3& first, const geox::Point3& second)
    {
        return first == second;
    }
} // namespace

GEOX_TEST(segment2_algorithms_handle_projection_membership_and_intersections)
{
    using namespace geox;

    const Segment2 horizontal{Point2{0.0, 0.0}, Point2{4.0, 0.0}};
    GEOX_REQUIRE(isPointOnSegment(Point2{2.0, 0.0}, horizontal));
    GEOX_REQUIRE(!isPointOnSegment(Point2{2.0, 0.1}, horizontal));
    GEOX_REQUIRE(samePoint(closestPointOnSegment(Point2{2.0, 3.0}, horizontal), Point2{2.0, 0.0}));
    GEOX_REQUIRE_CLOSE(squaredDistanceToSegment(Point2{2.0, 3.0}, horizontal), 9.0);

    const SegmentIntersection2 crossing = intersect(
        Segment2{Point2{0.0, 0.0}, Point2{4.0, 4.0}},
        Segment2{Point2{0.0, 4.0}, Point2{4.0, 0.0}}
    );
    GEOX_REQUIRE(std::holds_alternative<Point2>(crossing));
    GEOX_REQUIRE(samePoint(std::get<Point2>(crossing), Point2{2.0, 2.0}));

    const SegmentIntersection2 overlap = intersect(
        horizontal,
        Segment2{Point2{3.0, 0.0}, Point2{1.0, 0.0}}
    );
    GEOX_REQUIRE(std::holds_alternative<Segment2>(overlap));
    GEOX_REQUIRE(samePoint(std::get<Segment2>(overlap).start(), Point2{1.0, 0.0}));
    GEOX_REQUIRE(samePoint(std::get<Segment2>(overlap).end(), Point2{3.0, 0.0}));
    GEOX_REQUIRE(std::holds_alternative<std::monostate>(intersect(
        horizontal,
        Segment2{Point2{0.0, 1.0}, Point2{4.0, 1.0}}
    )));
}

GEOX_TEST(segment3_algorithms_reject_skew_segments_and_handle_collinearity)
{
    using namespace geox;

    const Segment3 horizontal{Point3{}, Point3{2.0, 0.0, 0.0}};
    GEOX_REQUIRE(isPointOnSegment(Point3{1.0, 0.0, 0.0}, horizontal));
    GEOX_REQUIRE(!isPointOnSegment(Point3{1.0, 0.0, 0.1}, horizontal));
    GEOX_REQUIRE(samePoint(closestPointOnSegment(Point3{3.0, 1.0, 0.0}, horizontal), Point3{2.0, 0.0, 0.0}));
    GEOX_REQUIRE_CLOSE(squaredDistanceToSegment(Point3{1.0, 2.0, 3.0}, horizontal), 13.0);

    const SegmentIntersection3 crossing = intersect(
        horizontal,
        Segment3{Point3{1.0, -1.0, 0.0}, Point3{1.0, 1.0, 0.0}}
    );
    GEOX_REQUIRE(std::holds_alternative<Point3>(crossing));
    GEOX_REQUIRE(samePoint(std::get<Point3>(crossing), Point3{1.0, 0.0, 0.0}));
    GEOX_REQUIRE(std::holds_alternative<std::monostate>(intersect(
        horizontal,
        Segment3{Point3{1.0, -1.0, 1.0}, Point3{1.0, 1.0, 1.0}}
    )));

    const SegmentIntersection3 overlap = intersect(
        horizontal,
        Segment3{Point3{1.0, 0.0, 0.0}, Point3{3.0, 0.0, 0.0}}
    );
    GEOX_REQUIRE(std::holds_alternative<Segment3>(overlap));
    GEOX_REQUIRE(samePoint(std::get<Segment3>(overlap).end(), Point3{2.0, 0.0, 0.0}));
}

GEOX_TEST(triangle2_algorithms_calculate_barycentrics_and_handle_degenerate_input)
{
    using namespace geox;

    const Triangle2 triangle{Point2{}, Point2{4.0, 0.0}, Point2{0.0, 3.0}};
    const auto barycentric = barycentricCoordinates(Point2{1.0, 1.0}, triangle);
    GEOX_REQUIRE(barycentric.has_value());
    GEOX_REQUIRE_CLOSE(barycentric->l0, 5.0 / 12.0);
    GEOX_REQUIRE_CLOSE(barycentric->l1, 0.25);
    GEOX_REQUIRE_CLOSE(barycentric->l2, 1.0 / 3.0);
    GEOX_REQUIRE(isPointInTriangle(Point2{0.0, 0.0}, triangle));
    GEOX_REQUIRE(!isPointInTriangle(Point2{4.0, 3.0}, triangle));

    const Triangle2 flat{Point2{}, Point2{1.0, 1.0}, Point2{2.0, 2.0}};
    GEOX_REQUIRE(!barycentricCoordinates(Point2{1.0, 1.0}, flat).has_value());
    GEOX_REQUIRE(!isPointInTriangle(Point2{1.0, 1.0}, flat));
}

GEOX_TEST(triangle3_algorithms_check_the_plane_and_find_closest_points)
{
    using namespace geox;

    const Triangle3 triangle{Point3{}, Point3{1.0, 0.0, 0.0}, Point3{0.0, 1.0, 0.0}};
    const auto barycentric = barycentricCoordinates(Point3{0.25, 0.25, 0.0}, triangle);
    GEOX_REQUIRE(barycentric.has_value());
    GEOX_REQUIRE_CLOSE(barycentric->l0, 0.5);
    GEOX_REQUIRE(isPointInTriangle(Point3{0.25, 0.25, 0.0}, triangle));
    GEOX_REQUIRE(!barycentricCoordinates(Point3{0.25, 0.25, 0.1}, triangle).has_value());
    GEOX_REQUIRE(!isPointInTriangle(Point3{0.25, 0.25, 0.1}, triangle));
    GEOX_REQUIRE(samePoint(
        closestPointOnTriangle(Point3{0.25, 0.25, 2.0}, triangle),
        Point3{0.25, 0.25, 0.0}
    ));
    GEOX_REQUIRE(samePoint(
        closestPointOnEdges(Point3{0.25, 0.25, 2.0}, triangle),
        Point3{0.25, 0.0, 0.0}
    ));
    GEOX_REQUIRE(samePoint(
        closestPointOnTriangle(Point3{0.8, 0.8, 2.0}, triangle),
        Point3{0.5, 0.5, 0.0}
    ));
}
