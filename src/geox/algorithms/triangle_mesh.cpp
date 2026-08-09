#include "geox/algorithms/triangle_mesh.h"

#include "geox/algorithms/triangle3.h"
#include "geox/primitives/triangle_mesh.h"

#include <stdexcept>

namespace geox
{

    std::optional<ClosestPointOnMeshResult> closestPointOnMesh(
        const Point3& point,
        const TriangleMesh& mesh
    )
    {
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot query an invalid triangle mesh.");

        std::optional<ClosestPointOnMeshResult> result;

        for (TriangleId triangleId = 0; triangleId < mesh.triangleCount(); ++triangleId)
        {
            const TriangleIndices& indices = mesh.triangle(triangleId);
            const Triangle3 triangle{
                mesh.vertex(indices[0]),
                mesh.vertex(indices[1]),
                mesh.vertex(indices[2])
            };
            const Point3 closestPoint = closestPointOnTriangle(point, triangle);
            const double squaredDistance = point.squaredDistance(closestPoint);

            if (!result.has_value() || squaredDistance < result->squaredDistance)
            {
                result = ClosestPointOnMeshResult{
                    triangleId,
                    closestPoint,
                    squaredDistance
                };
            }
        }

        return result;
    }

} // namespace geox
