#pragma once

#include "geox/dim2/primitives/point.h"

namespace geox::dim2
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

} // namespace geox::dim2
