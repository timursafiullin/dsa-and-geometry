#pragma once

#include "geox/primitives/segments.h"
#include "geox/constants.h"

#include <variant>

namespace geox
{

    // Returns whether point is within linearEpsilon distance of segment.
    bool isPointOnSegment(
        const Point3& point,
        const Segment3& segment,
        double linearEpsilon = kLinearEpsilon
    );

    Point3 closestPointOnSegment(
        const Point3& point,
        const Segment3& segment
    );

    double squaredDistanceToSegment(
        const Point3& point,
        const Segment3& segment
    );

    using SegmentIntersection3 = std::variant<
        std::monostate, // The segments do not intersect.
        Point3,         // The segments intersect at exactly one point.
        Segment3        // The collinear segments overlap along this segment.
    >;

    SegmentIntersection3 intersect(
        const Segment3& first,
        const Segment3& second,
        double linearEpsilon = kLinearEpsilon
    );

} // namespace geox
