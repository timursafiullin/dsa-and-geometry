#pragma once

#include "geometry/primitives/points.h"
#include "geometry/primitives/triangles.h"
#include "geometry/predicates.h"
#include "geometry/constants.h"

#include <optional>

namespace dsa::geometry
{

    struct BarycentricCoordinates2
    {
        double l0;
        double l1;
        double l2;
    };

    // Returns nullopt when the triangle is degenerate according to the
    // dimensionless relative degeneracyEpsilon.
    std::optional<BarycentricCoordinates2> barycentricCoordinates(
        const Point2& point,
        const Triangle2& triangle,
        double degeneracyEpsilon = kRelativeEpsilon
    );

    // Returns whether point lies inside triangle or within barycentricEpsilon
    // of its boundary. Degenerate triangles return false.
    bool isPointInTriangle(
        const Point2& point,
        const Triangle2& triangle,
        double barycentricEpsilon = kBarycentricEpsilon,
        double degeneracyEpsilon = kRelativeEpsilon
    );

} // namespace dsa::geometry
