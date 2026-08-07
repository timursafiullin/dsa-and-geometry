#pragma once

#include "geometry/primitives/points.h"
#include "geometry/primitives/triangles.h"
#include "geometry/predicates.h"
#include "geometry/constants.h"

#include <optional>

namespace dsa::geometry
{

    struct BarycentricCoordinates3
    {
        double l0;
        double l1;
        double l2;
    };

    // Returns nullopt for a degenerate triangle or a point outside its plane.
    std::optional<BarycentricCoordinates3> barycentricCoordinates(
        const Point3& point,
        const Triangle3& triangle,
        double degeneracyEpsilon = kRelativeEpsilon,
        double planeEpsilon = kLinearEpsilon
    );

    // Returns whether a coplanar point is inside the triangle, including its boundary.
    bool isPointInTriangle(
        const Point3& point,
        const Triangle3& triangle,
        double barycentricEpsilon = kBarycentricEpsilon,
        double degeneracyEpsilon = kRelativeEpsilon,
        double planeEpsilon = kLinearEpsilon
    );

    // Returns the nearest point on one of the triangle's three edges.
    Point3 closestPointOnEdges(
        const Point3& point,
        const Triangle3& triangle
    );

    // Returns the nearest point on triangle.
    Point3 closestPointOnTriangle(
        const Point3& point,
        const Triangle3& triangle
    );

} // namespace dsa::geometry
