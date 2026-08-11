#include "geox/dim2/spatial/bounds.h"

#include <algorithm>
#include <stdexcept>

namespace
{
    geox::dim2::AABB boundsFromPoints(
        const geox::dim2::Point& first,
        const geox::dim2::Point& second
    )
    {
        return {
            geox::dim2::Point{
                std::min(first.x(), second.x()),
                std::min(first.y(), second.y())
            },
            geox::dim2::Point{
                std::max(first.x(), second.x()),
                std::max(first.y(), second.y())
            }
        };
    }

    geox::dim2::AABB boundsFromPoints(
        const geox::dim2::Point& first,
        const geox::dim2::Point& second,
        const geox::dim2::Point& third
    )
    {
        return {
            geox::dim2::Point{
                std::min({first.x(), second.x(), third.x()}),
                std::min({first.y(), second.y(), third.y()})
            },
            geox::dim2::Point{
                std::max({first.x(), second.x(), third.x()}),
                std::max({first.y(), second.y(), third.y()})
            }
        };
    }
} // namespace

namespace geox::dim2
{

    AABB boundingBox(const Segment& segment)
    {
        return boundsFromPoints(segment.start(), segment.end());
    }

    AABB boundingBox(const Triangle& triangle)
    {
        return boundsFromPoints(triangle.a(), triangle.b(), triangle.c());
    }

} // namespace geox::dim2
