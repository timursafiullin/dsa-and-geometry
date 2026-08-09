#include "geox/spatial/aabb.h"

#include <algorithm>
#include <stdexcept>

namespace
{
    void validateBounds(const geox::Point2& minPoint, const geox::Point2& maxPoint)
    {
        for (int axis = 0; axis < 2; ++axis)
        {
            if (!(minPoint[axis] <= maxPoint[axis]))
                throw std::invalid_argument("AABB minimum must not exceed maximum.");
        }
    }

    void validateBounds(const geox::Point3& minPoint, const geox::Point3& maxPoint)
    {
        for (int axis = 0; axis < 3; ++axis)
        {
            if (!(minPoint[axis] <= maxPoint[axis]))
                throw std::invalid_argument("AABB minimum must not exceed maximum.");
        }
    }
} // namespace

namespace geox
{

    // MARK: AABB2
    AABB2::AABB2(const Point2& minPoint, const Point2& maxPoint)
        : min_(minPoint), max_(maxPoint)
    {
        validateBounds(min_, max_);
    }

    const Point2& AABB2::min() const noexcept { return min_; }
    const Point2& AABB2::max() const noexcept { return max_; }

    Point2 AABB2::center() const
    {
        return min_ + 0.5 * extent();
    }

    vector2d AABB2::extent() const
    {
        return max_ - min_;
    }

    bool AABB2::contains(const Point2& point) const noexcept
    {
        return point.x() >= min_.x() && point.x() <= max_.x() &&
               point.y() >= min_.y() && point.y() <= max_.y();
    }

    bool AABB2::overlaps(const AABB2& other) const noexcept
    {
        return min_.x() <= other.max_.x() && other.min_.x() <= max_.x() &&
               min_.y() <= other.max_.y() && other.min_.y() <= max_.y();
    }

    double AABB2::squaredDistance(const Point2& point) const noexcept
    {
        const double dx = std::max({min_.x() - point.x(), 0.0, point.x() - max_.x()});
        const double dy = std::max({min_.y() - point.y(), 0.0, point.y() - max_.y()});
        return dx * dx + dy * dy;
    }

    std::size_t AABB2::longestAxis() const noexcept
    {
        const vector2d boxExtent = extent();
        return boxExtent.y() > boxExtent.x() ? 1 : 0;
    }

    // MARK: AABB3
    AABB3::AABB3(const Point3& minPoint, const Point3& maxPoint)
        : min_(minPoint), max_(maxPoint)
    {
        validateBounds(min_, max_);
    }

    const Point3& AABB3::min() const noexcept { return min_; }
    const Point3& AABB3::max() const noexcept { return max_; }

    Point3 AABB3::center() const
    {
        return min_ + 0.5 * extent();
    }

    vector3d AABB3::extent() const
    {
        return max_ - min_;
    }

    bool AABB3::contains(const Point3& point) const noexcept
    {
        return point.x() >= min_.x() && point.x() <= max_.x() &&
               point.y() >= min_.y() && point.y() <= max_.y() &&
               point.z() >= min_.z() && point.z() <= max_.z();
    }

    bool AABB3::overlaps(const AABB3& other) const noexcept
    {
        return min_.x() <= other.max_.x() && other.min_.x() <= max_.x() &&
               min_.y() <= other.max_.y() && other.min_.y() <= max_.y() &&
               min_.z() <= other.max_.z() && other.min_.z() <= max_.z();
    }

    double AABB3::squaredDistance(const Point3& point) const noexcept
    {
        const double dx = std::max({min_.x() - point.x(), 0.0, point.x() - max_.x()});
        const double dy = std::max({min_.y() - point.y(), 0.0, point.y() - max_.y()});
        const double dz = std::max({min_.z() - point.z(), 0.0, point.z() - max_.z()});
        return dx * dx + dy * dy + dz * dz;
    }

    std::size_t AABB3::longestAxis() const noexcept
    {
        const vector3d boxExtent = extent();

        if (boxExtent.y() > boxExtent.x() && boxExtent.y() >= boxExtent.z())
            return 1;

        return boxExtent.z() > boxExtent.x() ? 2 : 0;
    }

    AABB2 merge(const AABB2& first, const AABB2& second)
    {
        return {
            Point2{
                std::min(first.min().x(), second.min().x()),
                std::min(first.min().y(), second.min().y())
            },
            Point2{
                std::max(first.max().x(), second.max().x()),
                std::max(first.max().y(), second.max().y())
            }
        };
    }

    AABB3 merge(const AABB3& first, const AABB3& second)
    {
        return {
            Point3{
                std::min(first.min().x(), second.min().x()),
                std::min(first.min().y(), second.min().y()),
                std::min(first.min().z(), second.min().z())
            },
            Point3{
                std::max(first.max().x(), second.max().x()),
                std::max(first.max().y(), second.max().y()),
                std::max(first.max().z(), second.max().z())
            }
        };
    }

} // namespace geox
