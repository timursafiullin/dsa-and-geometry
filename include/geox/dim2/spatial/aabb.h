#pragma once

#include "geox/dim2/primitives/point.h"

#include <cstddef>

namespace geox::dim2
{
    class AABB
    {
    public:
        // Throws std::invalid_argument unless every min coordinate is at most max.
        AABB(const Point& minPoint, const Point& maxPoint);

        const Point& min() const noexcept;
        const Point& max() const noexcept;

        Point center() const;
        Vector extent() const;

        bool contains(const Point& point) const noexcept;
        bool overlaps(const AABB& other) const noexcept;
        double squaredDistance(const Point& point) const noexcept;
        std::size_t longestAxis() const noexcept;

    private:
        Point min_;
        Point max_;
    };

    AABB merge(const AABB& first, const AABB& second);
} // namespace geox::dim2
