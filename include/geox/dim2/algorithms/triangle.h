#pragma once

#include "geox/core/constants.h"
#include "geox/dim2/primitives/triangle.h"

#include <optional>

namespace geox::dim2
{

    struct BarycentricCoordinates
    {
        double l0;
        double l1;
        double l2;
    };

    // Returns nullopt when the triangle is degenerate according to the
    // dimensionless relative degeneracyEpsilon.
    std::optional<BarycentricCoordinates> barycentricCoordinates(
        const Point& point,
        const Triangle& triangle,
        double degeneracyEpsilon = core::kRelativeEpsilon
    );

    // Returns whether point lies inside triangle or within barycentricEpsilon
    // of its boundary. Degenerate triangles return false.
    bool isPointInTriangle(
        const Point& point,
        const Triangle& triangle,
        double barycentricEpsilon = core::kBarycentricEpsilon,
        double degeneracyEpsilon = core::kRelativeEpsilon
    );

} // namespace geox::dim2
