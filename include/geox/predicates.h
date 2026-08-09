#pragma once

#include "geox/primitives/points.h"
#include <geox/constants.h>

namespace geox
{

    // Twice the signed area of triangle (a, b, c).
    //
    // The sign of the result determines the position of c relative to
    // the directed line a -> b:
    //   > 0 — c is to the left;
    //   < 0 — c is to the right;
    //   = 0 — a, b, and c are collinear.
    double orient2d(const Point2& a, const Point2& b, const Point2& c);

} // namespace geox
