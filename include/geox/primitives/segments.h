#pragma once

#include "geox/primitives/vectors.h"
#include "geox/primitives/points.h"

namespace geox
{

    class Segment2
    {
    public:
        Segment2(const Point2& start, const Point2& end);

        const Point2& start() const;
        const Point2& end() const;

        vector2d direction() const;
        double length() const;
        Point2 pointAt(double t) const;

    private:
        Point2 start_;
        Point2 end_;
    };

    class Segment3
    {
    public:
        Segment3(const Point3& start, const Point3& end);

        const Point3& start() const;
        const Point3& end() const;

        vector3d direction() const;
        double length() const;
        Point3 pointAt(double t) const;

    private:
        Point3 start_;
        Point3 end_;
    };

} // namespace geox