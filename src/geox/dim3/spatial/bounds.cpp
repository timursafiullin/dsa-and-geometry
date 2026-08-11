#include "geox/dim3/spatial/bounds.h"

#include <algorithm>
#include <stdexcept>

namespace
{
    geox::dim3::AABB boundsFromPoints(
        const geox::dim3::Point& first,
        const geox::dim3::Point& second
    )
    {
        return {
            geox::dim3::Point{
                std::min(first.x(), second.x()),
                std::min(first.y(), second.y()),
                std::min(first.z(), second.z())
            },
            geox::dim3::Point{
                std::max(first.x(), second.x()),
                std::max(first.y(), second.y()),
                std::max(first.z(), second.z())
            }
        };
    }

    geox::dim3::AABB boundsFromPoints(
        const geox::dim3::Point& first,
        const geox::dim3::Point& second,
        const geox::dim3::Point& third
    )
    {
        return {
            geox::dim3::Point{
                std::min({first.x(), second.x(), third.x()}),
                std::min({first.y(), second.y(), third.y()}),
                std::min({first.z(), second.z(), third.z()})
            },
            geox::dim3::Point{
                std::max({first.x(), second.x(), third.x()}),
                std::max({first.y(), second.y(), third.y()}),
                std::max({first.z(), second.z(), third.z()})
            }
        };
    }
} // namespace

namespace geox::dim3
{

    AABB boundingBox(const Segment& segment)
    {
        return boundsFromPoints(segment.start(), segment.end());
    }

    AABB boundingBox(const Triangle& triangle)
    {
        return boundsFromPoints(triangle.a(), triangle.b(), triangle.c());
    }

    AABB boundingBox(const TriangleMesh& mesh, TriangleId triangleId)
    {
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot calculate bounds for an invalid triangle mesh.");

        const TriangleIndices& indices = mesh.triangle(triangleId);
        return boundsFromPoints(
            mesh.vertex(indices[0]),
            mesh.vertex(indices[1]),
            mesh.vertex(indices[2])
        );
    }

    std::optional<AABB> boundingBox(const TriangleMesh& mesh)
    {
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot calculate bounds for an invalid triangle mesh.");

        if (mesh.triangleCount() == 0)
            return std::nullopt;

        AABB result = boundingBox(mesh, 0);
        for (TriangleId triangleId = 1; triangleId < mesh.triangleCount(); ++triangleId)
            result = merge(result, boundingBox(mesh, triangleId));

        return result;
    }

} // namespace geox::dim3
