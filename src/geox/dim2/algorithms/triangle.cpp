#include "geox/dim2/algorithms/triangle.h"

#include <cmath>

namespace geox::dim2
{

    std::optional<BarycentricCoordinates> barycentricCoordinates(
        const Point& point,
        const Triangle& triangle,
        double degeneracyEpsilon
    )
    {
        const Vector ab = triangle.b() - triangle.a();
        const Vector ac = triangle.c() - triangle.a();
        const Vector ap = point - triangle.a();

        const double d = ab.cross(ac);

        // Degenerate triangle
        if (std::abs(d) <= degeneracyEpsilon * ab.norm() * ac.norm()) return std::nullopt;

        const double l1 = ap.cross(ac) / d;
        const double l2 = ab.cross(ap) / d;
        const double l0 = 1.0 - l1 - l2;

        return BarycentricCoordinates{l0, l1, l2};
    }

    bool isPointInTriangle(
        const Point& point,
        const Triangle& triangle,
        double barycentricEpsilon,
        double degeneracyEpsilon
    )
    {
        const std::optional<BarycentricCoordinates> barycentric =
            barycentricCoordinates(point, triangle, degeneracyEpsilon);

        if (!barycentric.has_value()) return false;
        
        const BarycentricCoordinates& bCoords = barycentric.value();

        return bCoords.l0 >= -barycentricEpsilon &&
               bCoords.l1 >= -barycentricEpsilon &&
               bCoords.l2 >= -barycentricEpsilon;
    }

} // namespace geox::dim2
