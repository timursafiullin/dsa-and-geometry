#pragma once

#include "geox/core/constants.h"
#include "geox/dim3/primitives/triangle.h"

#include <optional>

namespace geox::dim3
{

    struct BarycentricCoordinates
    {
        double l0;
        double l1;
        double l2;
    };

    // Returns nullopt for a degenerate triangle or a point outside its plane.
    std::optional<BarycentricCoordinates> barycentricCoordinates(
        const Point& point,
        const Triangle& triangle,
        double degeneracyEpsilon = core::kRelativeEpsilon,
        double planeEpsilon = core::kLinearEpsilon
    );

    // Returns whether a coplanar point is inside the triangle, including its boundary.
    bool isPointInTriangle(
        const Point& point,
        const Triangle& triangle,
        double barycentricEpsilon = core::kBarycentricEpsilon,
        double degeneracyEpsilon = core::kRelativeEpsilon,
        double planeEpsilon = core::kLinearEpsilon
    );

    // Returns the nearest point on one of the triangle's three edges.
    Point closestPointOnEdges(
        const Point& point,
        const Triangle& triangle
    );

    // Returns the nearest point on triangle.
    Point closestPointOnTriangle(
        const Point& point,
        const Triangle& triangle
    );

} // namespace geox::dim3
