#pragma once

#include "geox/dim2/primitives/segment.h"
#include "geox/dim2/primitives/triangle.h"
#include "geox/dim2/spatial/aabb.h"

namespace geox::dim2
{
    AABB boundingBox(const Segment& segment);
    AABB boundingBox(const Triangle& triangle);
} // namespace geox::dim2
