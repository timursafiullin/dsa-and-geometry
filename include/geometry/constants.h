#pragma once

namespace dsa::geometry
{

    // Absolute tolerance for quantities measured in coordinate units.
    inline constexpr double kLinearEpsilon = 1e-10;

    // Relative tolerance for normalized geometric quantities, such as
    // the sine of the angle between two edges.
    inline constexpr double kRelativeEpsilon = 1e-10;

    // Tolerance for dimensionless barycentric coordinates.
    inline constexpr double kBarycentricEpsilon = 1e-10;

} // namespace dsa::geometry
