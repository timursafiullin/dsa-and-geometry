#pragma once

#include "geox/mesh_ids.h"
#include "geox/spatial/aabb.h"

#include <optional>

namespace geox
{

    class Segment2;
    class Segment3;
    class Triangle2;
    class Triangle3;
    class TriangleMesh;

    AABB2 boundingBox(
        const Segment2& segment
    );

    AABB3 boundingBox(
        const Segment3& segment
    );

    AABB2 boundingBox(
        const Triangle2& triangle
    );

    AABB3 boundingBox(
        const Triangle3& triangle
    );

    AABB3 boundingBox(
        const TriangleMesh& mesh,
        TriangleId triangleId
    );

    std::optional<AABB3> boundingBox(
        const TriangleMesh& mesh
    );

} // namespace geox