#include "geox/dim3/spatial/aabb.h"

#include <algorithm>
#include <stdexcept>

namespace
{
    void validateBounds(const geox::dim3::Point& minPoint, const geox::dim3::Point& maxPoint)
    {
        for (int axis = 0; axis < 3; ++axis)
        {
            if (!(minPoint[axis] <= maxPoint[axis]))
                throw std::invalid_argument("AABB minimum must not exceed maximum.");
        }
    }
} // namespace

namespace geox::dim3
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
               point.y() >= min_.y() && point.y() <= max_.y() &&
               point.z() >= min_.z() && point.z() <= max_.z();
    }

    bool AABB::overlaps(const AABB& other) const noexcept
    {
        return min_.x() <= other.max_.x() && other.min_.x() <= max_.x() &&
               min_.y() <= other.max_.y() && other.min_.y() <= max_.y() &&
               min_.z() <= other.max_.z() && other.min_.z() <= max_.z();
    }

    double AABB::squaredDistance(const Point& point) const noexcept
    {
        const double dx = std::max({min_.x() - point.x(), 0.0, point.x() - max_.x()});
        const double dy = std::max({min_.y() - point.y(), 0.0, point.y() - max_.y()});
        const double dz = std::max({min_.z() - point.z(), 0.0, point.z() - max_.z()});
        return dx * dx + dy * dy + dz * dz;
    }

    std::size_t AABB::longestAxis() const noexcept
    {
        const Vector boxExtent = extent();

        if (boxExtent.y() > boxExtent.x() && boxExtent.y() >= boxExtent.z())
            return 1;

        return boxExtent.z() > boxExtent.x() ? 2 : 0;
    }

    AABB merge(const AABB& first, const AABB& second)
    {
        return {
            Point{
                std::min(first.min().x(), second.min().x()),
                std::min(first.min().y(), second.min().y()),
                std::min(first.min().z(), second.min().z())
            },
            Point{
                std::max(first.max().x(), second.max().x()),
                std::max(first.max().y(), second.max().y()),
                std::max(first.max().z(), second.max().z())
            }
        };
    }

} // namespace geox::dim3
