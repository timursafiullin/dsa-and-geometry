#include "geox/dim3/algorithms/segment.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace geox::dim3
{

    bool isPointOnSegment(
        const Point& point,
        const Segment& segment,
        double linearEpsilon
    )
    {
        const Point& a = segment.start();
        const Point& b = segment.end();
        const Vector direction = b - a;
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

    Point closestPointOnSegment(
        const Point& point,
        const Segment& segment
    )
    {
        const Point& a = segment.start();
        const Vector direction = segment.end() - a;
        const double directionSquaredNorm = direction.squaredNorm();

        if (directionSquaredNorm == 0.0) return a;

        const double t = (point - a).dot(direction) / directionSquaredNorm;
        return segment.pointAt(std::clamp(t, 0.0, 1.0));
    }

    double squaredDistanceToSegment(
        const Point& point,
        const Segment& segment
    )
    {
        return point.squaredDistance(closestPointOnSegment(point, segment));
    }

    SegmentIntersection intersect(
        const Segment& first,
        const Segment& second,
        double linearEpsilon
    )
    {
        const Point& a = first.start();
        const Point& b = first.end();
        const Point& c = second.start();
        const Point& d = second.end();

        const Vector r = b - a;
        const Vector s = d - c;
        const Vector cMinusA = c - a;
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
                    ? SegmentIntersection{a}
                    : SegmentIntersection{std::monostate{}};
            }

            return isPointOnSegment(a, second, linearEpsilon)
                ? SegmentIntersection{a}
                : SegmentIntersection{std::monostate{}};
        }

        if (sSquaredNorm <= linearEpsilon * linearEpsilon)
        {
            return isPointOnSegment(c, first, linearEpsilon)
                ? SegmentIntersection{c}
                : SegmentIntersection{std::monostate{}};
        }

        const Vector rCrossS = r.cross(s);
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

            const Point pointOnFirst = first.pointAt(std::clamp(t, 0.0, 1.0));
            const Point pointOnSecond = second.pointAt(std::clamp(u, 0.0, 1.0));
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

        return Segment{
            first.pointAt(overlapStart),
            first.pointAt(overlapEnd)
        };
    }

} // namespace geox::dim3
