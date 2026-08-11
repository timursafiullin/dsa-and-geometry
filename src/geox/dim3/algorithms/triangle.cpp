#include "geox/dim3/algorithms/triangle.h"
#include "geox/dim3/algorithms/segment.h"

#include <cmath>

namespace geox::dim3
{

    std::optional<BarycentricCoordinates> barycentricCoordinates(
        const Point& point,
        const Triangle& triangle,
        double degeneracyEpsilon,
        double planeEpsilon
    )
    {
        const Vector ab = triangle.b() - triangle.a();
        const Vector ac = triangle.c() - triangle.a();
        const Vector ap = point - triangle.a();

        const Vector n = ab.cross(ac);
        const double nLength = n.norm();

        // Degenerate triangle
        if (nLength <= degeneracyEpsilon * ab.norm() * ac.norm()) return std::nullopt;

        // The signed distance to the plane is dot(ap, n) / |n|.
        if (std::abs(ap.dot(n)) > planeEpsilon * nLength) return std::nullopt;

        const double l1 = n.dot(ap.cross(ac)) / n.squaredNorm();
        const double l2 = n.dot(ab.cross(ap)) / n.squaredNorm();
        const double l0 = 1 - l1 - l2;

        return BarycentricCoordinates{l0, l1, l2};
    }

    bool isPointInTriangle(
        const Point& point,
        const Triangle& triangle,
        double barycentricEpsilon,
        double degeneracyEpsilon,
        double planeEpsilon
    )
    {
        const std::optional<BarycentricCoordinates> barycentric =
            barycentricCoordinates(point, triangle, degeneracyEpsilon, planeEpsilon);

        if (!barycentric.has_value()) return false;
        
        const BarycentricCoordinates& bCoords = barycentric.value();

        return bCoords.l0 >= -barycentricEpsilon &&
               bCoords.l1 >= -barycentricEpsilon &&
               bCoords.l2 >= -barycentricEpsilon;
    }

    Point closestPointOnEdges(
        const Point& point,
        const Triangle& triangle
    )
    {
        Point closest = closestPointOnSegment(
            point, Segment{triangle.a(), triangle.b()});
        double closestSquaredDistance = point.squaredDistance(closest);

        const Point onBC = closestPointOnSegment(
            point, Segment{triangle.b(), triangle.c()});
        const double squaredDistanceToBC = point.squaredDistance(onBC);
        if (squaredDistanceToBC < closestSquaredDistance)
        {
            closest = onBC;
            closestSquaredDistance = squaredDistanceToBC;
        }

        const Point onCA = closestPointOnSegment(
            point, Segment{triangle.c(), triangle.a()});
        if (point.squaredDistance(onCA) < closestSquaredDistance)
            closest = onCA;

        return closest;
    }

    Point closestPointOnTriangle(
        const Point& point,
        const Triangle& triangle
    )
    {
        const Vector normal = triangle.areaNormal();
        const double normalSquaredNorm = normal.squaredNorm();

        // A plane is undefined for a degenerate triangle, so its edges are
        // the available geometric support.
        if (normalSquaredNorm == 0.0)
            return closestPointOnEdges(point, triangle);

        const Vector pointFromA = point - triangle.a();
        const Point projection =
            point - normal * (pointFromA.dot(normal) / normalSquaredNorm);

        if (isPointInTriangle(projection, triangle))
            return projection;

        return closestPointOnEdges(point, triangle);
    }

} // namespace geox::dim3
