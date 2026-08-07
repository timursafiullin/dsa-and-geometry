#pragma once

#include "geometry/primitives/segments.h"
#include "geometry/predicates.h"
#include "geometry/constants.h"

#include <variant>

namespace dsa::geometry
{

    // Returns whether point is within epsilon distance of segment.
    bool isPointOnSegment(
        const Point2& point,
        const Segment2& segment,
        double epsilon = kEpsilon
    );

    Point2 closestPointOnSegment(
        const Point2& point,
        const Segment2& segment
    );

    double squaredDistanceToSegment(
        const Point2& point,
        const Segment2& segment
    );
    
    using SegmentIntersection2 = std::variant<
        std::monostate, // The segments do not intersect.
        Point2,         // The segments intersect at exactly one point.
        Segment2        // The collinear segments overlap along this segment.
    >;

    SegmentIntersection2 intersect(
        const Segment2& first,
        const Segment2& second,
        double epsilon = kEpsilon
    );

} // namespace dsa::geometry
