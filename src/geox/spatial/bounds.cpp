#include "geox/spatial/bounds.h"

#include "geox/primitives/segments.h"
#include "geox/primitives/triangle_mesh.h"
#include "geox/primitives/triangles.h"

#include <algorithm>
#include <stdexcept>

namespace
{
    geox::AABB2 boundsFromPoints(
        const geox::Point2& first,
        const geox::Point2& second
    )
    {
        return {
            geox::Point2{
                std::min(first.x(), second.x()),
                std::min(first.y(), second.y())
            },
            geox::Point2{
                std::max(first.x(), second.x()),
                std::max(first.y(), second.y())
            }
        };
    }

    geox::AABB2 boundsFromPoints(
        const geox::Point2& first,
        const geox::Point2& second,
        const geox::Point2& third
    )
    {
        return {
            geox::Point2{
                std::min({first.x(), second.x(), third.x()}),
                std::min({first.y(), second.y(), third.y()})
            },
            geox::Point2{
                std::max({first.x(), second.x(), third.x()}),
                std::max({first.y(), second.y(), third.y()})
            }
        };
    }

    geox::AABB3 boundsFromPoints(
        const geox::Point3& first,
        const geox::Point3& second
    )
    {
        return {
            geox::Point3{
                std::min(first.x(), second.x()),
                std::min(first.y(), second.y()),
                std::min(first.z(), second.z())
            },
            geox::Point3{
                std::max(first.x(), second.x()),
                std::max(first.y(), second.y()),
                std::max(first.z(), second.z())
            }
        };
    }

    geox::AABB3 boundsFromPoints(
        const geox::Point3& first,
        const geox::Point3& second,
        const geox::Point3& third
    )
    {
        return {
            geox::Point3{
                std::min({first.x(), second.x(), third.x()}),
                std::min({first.y(), second.y(), third.y()}),
                std::min({first.z(), second.z(), third.z()})
            },
            geox::Point3{
                std::max({first.x(), second.x(), third.x()}),
                std::max({first.y(), second.y(), third.y()}),
                std::max({first.z(), second.z(), third.z()})
            }
        };
    }
} // namespace

namespace geox
{

    AABB2 boundingBox(const Segment2& segment)
    {
        return boundsFromPoints(segment.start(), segment.end());
    }

    AABB3 boundingBox(const Segment3& segment)
    {
        return boundsFromPoints(segment.start(), segment.end());
    }

    AABB2 boundingBox(const Triangle2& triangle)
    {
        return boundsFromPoints(triangle.a(), triangle.b(), triangle.c());
    }

    AABB3 boundingBox(const Triangle3& triangle)
    {
        return boundsFromPoints(triangle.a(), triangle.b(), triangle.c());
    }

    AABB3 boundingBox(const TriangleMesh& mesh, TriangleId triangleId)
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

    std::optional<AABB3> boundingBox(const TriangleMesh& mesh)
    {
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot calculate bounds for an invalid triangle mesh.");

        if (mesh.triangleCount() == 0)
            return std::nullopt;

        AABB3 result = boundingBox(mesh, 0);
        for (TriangleId triangleId = 1; triangleId < mesh.triangleCount(); ++triangleId)
            result = merge(result, boundingBox(mesh, triangleId));

        return result;
    }

} // namespace geox
