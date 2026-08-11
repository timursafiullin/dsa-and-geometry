#pragma once

#include "geox/dim2/primitives/point.h"

namespace geox::dim2
{

    // Twice the signed area of triangle (a, b, c).
    //
    // The sign of the result determines the position of c relative to
    // the directed line a -> b:
    //   > 0 — c is to the left;
    //   < 0 — c is to the right;
    //   = 0 — a, b, and c are collinear.
    double orient2d(const Point& a, const Point& b, const Point& c);

} // namespace geox::dim2
