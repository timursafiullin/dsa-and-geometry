#pragma once

#include "geox/dim3/primitives/point.h"

namespace geox::dim3
{

    class Segment
    {
    public:
        Segment(const Point& start, const Point& end);

        const Point& start() const;
        const Point& end() const;

        Vector direction() const;
        double length() const;
        Point pointAt(double t) const;

    private:
        Point start_;
        Point end_;
    };

} // namespace geox::dim3
