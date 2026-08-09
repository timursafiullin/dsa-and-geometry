#pragma once

#include "geox/primitives/points.h"
#include "geox/primitives/vectors.h"

#include <cstddef>

namespace geox
{

    class AABB2
    {
    public:
        // Throws std::invalid_argument unless every min coordinate is at most max.
        AABB2(const Point2& minPoint, const Point2& maxPoint);

        const Point2& min() const noexcept;
        const Point2& max() const noexcept;

        Point2 center() const;
        vector2d extent() const;

        bool contains(const Point2& point) const noexcept;
        bool overlaps(const AABB2& other) const noexcept;

        double squaredDistance(const Point2& point) const noexcept;

        std::size_t longestAxis() const noexcept;

    private:
        Point2 min_;
        Point2 max_;
    };

    class AABB3
    {
    public:
        // Throws std::invalid_argument unless every min coordinate is at most max.
        AABB3(const Point3& minPoint, const Point3& maxPoint);

        const Point3& min() const noexcept;
        const Point3& max() const noexcept;

        Point3 center() const;
        vector3d extent() const;

        bool contains(const Point3& point) const noexcept;
        bool overlaps(const AABB3& other) const noexcept;

        double squaredDistance(const Point3& point) const noexcept;

        std::size_t longestAxis() const noexcept;

    private:
        Point3 min_;
        Point3 max_;
    };

    AABB2 merge(
        const AABB2& first,
        const AABB2& second
    );

    AABB3 merge(
        const AABB3& first,
        const AABB3& second
    );

} // namespace geox
