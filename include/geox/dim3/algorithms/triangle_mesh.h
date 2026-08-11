#pragma once

#include "geox/dim3/mesh/triangle_mesh.h"

#include <optional>

namespace geox::dim3
{

    struct ClosestPointOnMeshResult
    {
        TriangleId triangleId;
        Point closestPoint;
        double squaredDistance;
    };

    // Exact linear scan over all triangles. Throws std::invalid_argument for
    // an invalid mesh. Equal-distance triangles are resolved by the lowest ID.
    // Complexity: O(number of triangles).
    std::optional<ClosestPointOnMeshResult> closestPointOnMesh(
        const Point& point,
        const TriangleMesh& mesh
    );

} // namespace geox::dim3
