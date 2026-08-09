#pragma once

#include "geox/mesh_ids.h"
#include "geox/primitives/points.h"

#include <optional>

namespace geox
{

    class TriangleMesh;

    struct ClosestPointOnMeshResult
    {
        TriangleId triangleId;
        Point3 closestPoint;
        double squaredDistance;
    };

    // Exact linear scan over all triangles. Throws std::invalid_argument for
    // an invalid mesh. Equal-distance triangles are resolved by the lowest ID.
    // Complexity: O(number of triangles).
    std::optional<ClosestPointOnMeshResult> closestPointOnMesh(
        const Point3& point,
        const TriangleMesh& mesh
    );

} // namespace geox
