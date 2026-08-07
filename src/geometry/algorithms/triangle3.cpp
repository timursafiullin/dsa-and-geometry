#include "geometry/algorithms/triangle3.h"

#include <cmath>

namespace dsa::geometry
{

    std::optional<BarycentricCoordinates3> barycentricCoordinates(
        const Point3& point,
        const Triangle3& triangle,
        double degeneracyEpsilon,
        double planeEpsilon
    )
    {
        const vector3d ab = triangle.b() - triangle.a();
        const vector3d ac = triangle.c() - triangle.a();
        const vector3d ap = point - triangle.a();

        const vector3d n = ab.cross(ac);
        const double nLength = n.norm();

        // Degenerate triangle
        if (nLength <= degeneracyEpsilon * ab.norm() * ac.norm()) return std::nullopt;

        // The signed distance to the plane is dot(ap, n) / |n|.
        if (std::abs(ap.dot(n)) > planeEpsilon * nLength) return std::nullopt;

        const double l1 = n.dot(ap.cross(ac)) / n.squaredNorm();
        const double l2 = n.dot(ab.cross(ap)) / n.squaredNorm();
        const double l0 = 1 - l1 - l2;

        return BarycentricCoordinates3{l0, l1, l2};
    }

    bool isPointInTriangle(
        const Point3& point,
        const Triangle3& triangle,
        double barycentricEpsilon,
        double degeneracyEpsilon,
        double planeEpsilon
    )
    {
        const std::optional<BarycentricCoordinates3> barycentric =
            barycentricCoordinates(point, triangle, degeneracyEpsilon, planeEpsilon);

        if (!barycentric.has_value()) return false;
        
        const BarycentricCoordinates3& bCoords = barycentric.value();

        return bCoords.l0 >= -barycentricEpsilon &&
               bCoords.l1 >= -barycentricEpsilon &&
               bCoords.l2 >= -barycentricEpsilon;
    }

} // namespace dsa::geometry
