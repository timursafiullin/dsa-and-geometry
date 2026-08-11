#pragma once

#include "geox/core/constants.h"
#include "geox/dim3/primitives/segment.h"

#include <variant>

namespace geox::dim3
{

    // Returns whether point is within linearEpsilon distance of segment.
    bool isPointOnSegment(
        const Point& point,
        const Segment& segment,
        double linearEpsilon = core::kLinearEpsilon
    );

    Point closestPointOnSegment(
        const Point& point,
        const Segment& segment
    );

    double squaredDistanceToSegment(
        const Point& point,
        const Segment& segment
    );

    using SegmentIntersection = std::variant<
        std::monostate, // The segments do not intersect.
        Point,         // The segments intersect at exactly one point.
        Segment        // The collinear segments overlap along this segment.
    >;

    SegmentIntersection intersect(
        const Segment& first,
        const Segment& second,
        double linearEpsilon = core::kLinearEpsilon
    );

} // namespace geox::dim3
