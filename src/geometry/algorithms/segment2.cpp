#include "geometry/algorithms/segment2.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace dsa::geometry
{

    bool isPointOnSegment(
        const Point2& point,
        const Segment2& segment,
        double epsilon
    )
    {
        const Point2& a = segment.start();
        const Point2& b = segment.end();
        const vector2d direction = b - a;
        const double directionSquaredNorm = direction.squaredNorm();

        if (directionSquaredNorm <= epsilon * epsilon)
        {
            return point.squaredDistance(a) <= epsilon * epsilon;
        }

        const double length = std::sqrt(directionSquaredNorm);
        return std::abs(orient2d(a, b, point)) <= epsilon * length &&
               (point - a).dot(point - b) <= epsilon * length;
    }

    Point2 closestPointOnSegment(
        const Point2& point,
        const Segment2& segment
    )
    {
        const Point2& a = segment.start();
        const Point2& b = segment.end();

        const vector2d v = b - a;
        const double vSquaredNorm = v.squaredNorm();

        if (vSquaredNorm == 0.0)
        {
            return a;
        }

        double t0 = (point - a).dot(v) / vSquaredNorm;

        if (t0 < 0) return a;
        if (t0 > 1) return b;
        else return segment.pointAt(t0);
    }

    double squaredDistanceToSegment(
        const Point2& point,
        const Segment2& segment
    )
    {
        const Point2 q = closestPointOnSegment(point, segment);
        return point.squaredDistance(q);
    }

    SegmentIntersection2 intersect(
        const Segment2& first,
        const Segment2& second,
        double epsilon
    )
    {
        const Point2& a = first.start();
        const Point2& b = first.end();
        const Point2& c = second.start();
        const Point2& d = second.end();

        const vector2d r = b - a;
        const vector2d s = d - c;
        const vector2d cMinusA = c - a;
        const double rSquaredNorm = r.squaredNorm();
        const double sSquaredNorm = s.squaredNorm();
        const double rLength = std::sqrt(rSquaredNorm);
        const double sLength = std::sqrt(sSquaredNorm);

        // Treat degenerate segments as points.
        if (rSquaredNorm <= epsilon * epsilon)
        {
            if (sSquaredNorm <= epsilon * epsilon)
            {
                return a.squaredDistance(c) <= epsilon * epsilon
                    ? SegmentIntersection2{a}
                    : SegmentIntersection2{std::monostate{}};
            }

            return isPointOnSegment(a, second, epsilon)
                ? SegmentIntersection2{a}
                : SegmentIntersection2{std::monostate{}};
        }

        if (sSquaredNorm <= epsilon * epsilon)
        {
            return isPointOnSegment(c, first, epsilon)
                ? SegmentIntersection2{c}
                : SegmentIntersection2{std::monostate{}};
        }

        const double rCrossS = r.cross(s);

        const double parallelTolerance =
            std::numeric_limits<double>::epsilon() * rLength * sLength;

        if (std::abs(rCrossS) > parallelTolerance)
        {
            const double t = cMinusA.cross(s) / rCrossS;
            const double u = cMinusA.cross(r) / rCrossS;

            if (t < -epsilon / rLength || t > 1.0 + epsilon / rLength ||
                u < -epsilon / sLength || u > 1.0 + epsilon / sLength)
            {
                return std::monostate{};
            }

            return first.pointAt(std::clamp(t, 0.0, 1.0));
        }

        // Parallel non-collinear segments do not intersect.
        if (std::abs(cMinusA.cross(r)) > epsilon * rLength)
        {
            return std::monostate{};
        }

        // The segments are collinear. Find the overlap in first's parameter space.
        const double tC = cMinusA.dot(r) / rSquaredNorm;
        const double tD = (d - a).dot(r) / rSquaredNorm;
        const double overlapStart = std::max(0.0, std::min(tC, tD));
        const double overlapEnd = std::min(1.0, std::max(tC, tD));

        if (overlapStart > overlapEnd + epsilon / rLength)
        {
            return std::monostate{};
        }

        if (std::abs(overlapEnd - overlapStart) <= epsilon / rLength)
        {
            return first.pointAt(std::clamp((overlapStart + overlapEnd) / 2.0, 0.0, 1.0));
        }

        return Segment2{
            first.pointAt(overlapStart),
            first.pointAt(overlapEnd)
        };
    }

} // namespace dsa::geometry
