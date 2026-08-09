#include "geox/algorithms/segment3.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace geox
{

    bool isPointOnSegment(
        const Point3& point,
        const Segment3& segment,
        double linearEpsilon
    )
    {
        const Point3& a = segment.start();
        const Point3& b = segment.end();
        const vector3d direction = b - a;
        const double directionSquaredNorm = direction.squaredNorm();

        if (directionSquaredNorm <= linearEpsilon * linearEpsilon)
            return point.squaredDistance(a) <= linearEpsilon * linearEpsilon;

        const double t = (point - a).dot(direction) / directionSquaredNorm;
        if (t < -linearEpsilon / std::sqrt(directionSquaredNorm) ||
            t > 1.0 + linearEpsilon / std::sqrt(directionSquaredNorm))
        {
            return false;
        }

        return point.squaredDistance(segment.pointAt(std::clamp(t, 0.0, 1.0))) <=
               linearEpsilon * linearEpsilon;
    }

    Point3 closestPointOnSegment(
        const Point3& point,
        const Segment3& segment
    )
    {
        const Point3& a = segment.start();
        const vector3d direction = segment.end() - a;
        const double directionSquaredNorm = direction.squaredNorm();

        if (directionSquaredNorm == 0.0) return a;

        const double t = (point - a).dot(direction) / directionSquaredNorm;
        return segment.pointAt(std::clamp(t, 0.0, 1.0));
    }

    double squaredDistanceToSegment(
        const Point3& point,
        const Segment3& segment
    )
    {
        return point.squaredDistance(closestPointOnSegment(point, segment));
    }

    SegmentIntersection3 intersect(
        const Segment3& first,
        const Segment3& second,
        double linearEpsilon
    )
    {
        const Point3& a = first.start();
        const Point3& b = first.end();
        const Point3& c = second.start();
        const Point3& d = second.end();

        const vector3d r = b - a;
        const vector3d s = d - c;
        const vector3d cMinusA = c - a;
        const double rSquaredNorm = r.squaredNorm();
        const double sSquaredNorm = s.squaredNorm();
        const double rLength = std::sqrt(rSquaredNorm);
        const double sLength = std::sqrt(sSquaredNorm);

        // Treat degenerate segments as points.
        if (rSquaredNorm <= linearEpsilon * linearEpsilon)
        {
            if (sSquaredNorm <= linearEpsilon * linearEpsilon)
            {
                return a.squaredDistance(c) <= linearEpsilon * linearEpsilon
                    ? SegmentIntersection3{a}
                    : SegmentIntersection3{std::monostate{}};
            }

            return isPointOnSegment(a, second, linearEpsilon)
                ? SegmentIntersection3{a}
                : SegmentIntersection3{std::monostate{}};
        }

        if (sSquaredNorm <= linearEpsilon * linearEpsilon)
        {
            return isPointOnSegment(c, first, linearEpsilon)
                ? SegmentIntersection3{c}
                : SegmentIntersection3{std::monostate{}};
        }

        const vector3d rCrossS = r.cross(s);
        const double rCrossSSquaredNorm = rCrossS.squaredNorm();
        const double parallelTolerance =
            std::numeric_limits<double>::epsilon() * rLength * sLength;

        if (rCrossSSquaredNorm > parallelTolerance * parallelTolerance)
        {
            const double t = cMinusA.cross(s).dot(rCrossS) / rCrossSSquaredNorm;
            const double u = cMinusA.cross(r).dot(rCrossS) / rCrossSSquaredNorm;

            if (t < -linearEpsilon / rLength || t > 1.0 + linearEpsilon / rLength ||
                u < -linearEpsilon / sLength || u > 1.0 + linearEpsilon / sLength)
            {
                return std::monostate{};
            }

            const Point3 pointOnFirst = first.pointAt(std::clamp(t, 0.0, 1.0));
            const Point3 pointOnSecond = second.pointAt(std::clamp(u, 0.0, 1.0));
            if (pointOnFirst.squaredDistance(pointOnSecond) > linearEpsilon * linearEpsilon)
                return std::monostate{};

            return pointOnFirst;
        }

        // Parallel non-collinear segments do not intersect.
        if (cMinusA.cross(r).norm() > linearEpsilon * rLength)
            return std::monostate{};

        // The segments are collinear. Find the overlap in first's parameter space.
        const double tC = cMinusA.dot(r) / rSquaredNorm;
        const double tD = (d - a).dot(r) / rSquaredNorm;
        const double overlapStart = std::max(0.0, std::min(tC, tD));
        const double overlapEnd = std::min(1.0, std::max(tC, tD));

        if (overlapStart > overlapEnd + linearEpsilon / rLength)
            return std::monostate{};

        if (std::abs(overlapEnd - overlapStart) <= linearEpsilon / rLength)
            return first.pointAt(std::clamp((overlapStart + overlapEnd) / 2.0, 0.0, 1.0));

        return Segment3{
            first.pointAt(overlapStart),
            first.pointAt(overlapEnd)
        };
    }

} // namespace geox
