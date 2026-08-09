#include "geox/algorithms/triangle2.h"

#include <cmath>

namespace geox
{

    std::optional<BarycentricCoordinates2> barycentricCoordinates(
        const Point2& point,
        const Triangle2& triangle,
        double degeneracyEpsilon
    )
    {
        const vector2d ab = triangle.b() - triangle.a();
        const vector2d ac = triangle.c() - triangle.a();
        const vector2d ap = point - triangle.a();

        const double d = ab.cross(ac);

        // Degenerate triangle
        if (std::abs(d) <= degeneracyEpsilon * ab.norm() * ac.norm()) return std::nullopt;

        const double l1 = ap.cross(ac) / d;
        const double l2 = ab.cross(ap) / d;
        const double l0 = 1.0 - l1 - l2;

        return BarycentricCoordinates2{l0, l1, l2};
    }

    bool isPointInTriangle(
        const Point2& point,
        const Triangle2& triangle,
        double barycentricEpsilon,
        double degeneracyEpsilon
    )
    {
        const std::optional<BarycentricCoordinates2> barycentric =
            barycentricCoordinates(point, triangle, degeneracyEpsilon);

        if (!barycentric.has_value()) return false;
        
        const BarycentricCoordinates2& bCoords = barycentric.value();

        return bCoords.l0 >= -barycentricEpsilon &&
               bCoords.l1 >= -barycentricEpsilon &&
               bCoords.l2 >= -barycentricEpsilon;
    }

} // namespace geox
