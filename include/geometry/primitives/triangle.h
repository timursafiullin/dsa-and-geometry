#pragma once

#include "geometry/primitives/vectors.h"
#include "geometry/primitives/points.h"

namespace dsa::geometry
{

    class Triangle2
    {
    public:
        Triangle2(
            const Point2& a,
            const Point2& b,
            const Point2& c
        );

        const Point2& a() const;
        const Point2& b() const;
        const Point2& c() const;

        vector2d edgeAB() const;
        vector2d edgeAC() const;

        double signedDoubleArea() const;
        double area() const;

        bool isDegenerate() const;

    private:
        Point2 a_;
        Point2 b_;
        Point2 c_;
    };

    class Triangle3
    {
    public:
        Triangle3(
            const Point3& a,
            const Point3& b,
            const Point3& c
        );

        const Point3& a() const;
        const Point3& b() const;
        const Point3& c() const;

        vector3d edgeAB() const;
        vector3d edgeAC() const;

        vector3d areaNormal() const;
        double area() const;
        vector3d normal() const;

        bool isDegenerate() const;

    private:
        Point3 a_;
        Point3 b_;
        Point3 c_;
    };

} // namespace dsa::geometry
