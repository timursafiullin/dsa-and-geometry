#include "geox/dim2/spatial/aabb.h"

#include <algorithm>
#include <stdexcept>

namespace
{
    void validateBounds(const geox::dim2::Point& minPoint, const geox::dim2::Point& maxPoint)
    {
        for (int axis = 0; axis < 2; ++axis)
        {
            if (!(minPoint[axis] <= maxPoint[axis]))
                throw std::invalid_argument("AABB minimum must not exceed maximum.");
        }
    }
} // namespace

namespace geox::dim2
{

    // MARK: AABB
    AABB::AABB(const Point& minPoint, const Point& maxPoint)
        : min_(minPoint), max_(maxPoint)
    {
        validateBounds(min_, max_);
    }

    const Point& AABB::min() const noexcept { return min_; }
    const Point& AABB::max() const noexcept { return max_; }

    Point AABB::center() const
    {
        return min_ + 0.5 * extent();
    }

    Vector AABB::extent() const
    {
        return max_ - min_;
    }

    bool AABB::contains(const Point& point) const noexcept
    {
        return point.x() >= min_.x() && point.x() <= max_.x() &&
               point.y() >= min_.y() && point.y() <= max_.y();
    }

    bool AABB::overlaps(const AABB& other) const noexcept
    {
        return min_.x() <= other.max_.x() && other.min_.x() <= max_.x() &&
               min_.y() <= other.max_.y() && other.min_.y() <= max_.y();
    }

    double AABB::squaredDistance(const Point& point) const noexcept
    {
        const double dx = std::max({min_.x() - point.x(), 0.0, point.x() - max_.x()});
        const double dy = std::max({min_.y() - point.y(), 0.0, point.y() - max_.y()});
        return dx * dx + dy * dy;
    }

    std::size_t AABB::longestAxis() const noexcept
    {
        const Vector boxExtent = extent();
        return boxExtent.y() > boxExtent.x() ? 1 : 0;
    }

    AABB merge(const AABB& first, const AABB& second)
    {
        return {
            Point{
                std::min(first.min().x(), second.min().x()),
                std::min(first.min().y(), second.min().y())
            },
            Point{
                std::max(first.max().x(), second.max().x()),
                std::max(first.max().y(), second.max().y())
            }
        };
    }

} // namespace geox::dim2
