#pragma once

#include "geox/dim3/mesh/triangle_mesh.h"
#include "geox/dim3/primitives/segment.h"
#include "geox/dim3/spatial/aabb.h"

#include <optional>

namespace geox::dim3
{
    AABB boundingBox(const Segment& segment);
    AABB boundingBox(const Triangle& triangle);
    AABB boundingBox(const TriangleMesh& mesh, TriangleId triangleId);
    std::optional<AABB> boundingBox(const TriangleMesh& mesh);
} // namespace geox::dim3
