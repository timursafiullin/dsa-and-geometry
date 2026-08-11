#include "geox/dim3/algorithms/triangle_mesh.h"

#include "geox/dim3/algorithms/triangle.h"
#include "geox/dim3/mesh/triangle_mesh.h"

#include <stdexcept>

namespace geox::dim3
{

    std::optional<ClosestPointOnMeshResult> closestPointOnMesh(
        const Point& point,
        const TriangleMesh& mesh
    )
    {
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot query an invalid triangle mesh.");

        std::optional<ClosestPointOnMeshResult> result;

        for (TriangleId triangleId = 0; triangleId < mesh.triangleCount(); ++triangleId)
        {
            const TriangleIndices& indices = mesh.triangle(triangleId);
            const Triangle triangle{
                mesh.vertex(indices[0]),
                mesh.vertex(indices[1]),
                mesh.vertex(indices[2])
            };
            const Point closestPoint = closestPointOnTriangle(point, triangle);
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

} // namespace geox::dim3
