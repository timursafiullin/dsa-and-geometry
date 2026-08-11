#pragma once

#include "geox/core/constants.h"
#include "geox/dim2/primitives/point.h"

namespace geox::dim2
{

    class Triangle
    {
    public:
        Triangle(const Point& a, const Point& b, const Point& c);

        const Point& a() const;
        const Point& b() const;
        const Point& c() const;

        Vector edgeAB() const;
        Vector edgeAC() const;

        double signedDoubleArea() const;
        double area() const;

        // relativeEpsilon bounds the sine of the angle between the edges.
        bool isDegenerate(double relativeEpsilon = core::kRelativeEpsilon) const;

    private:
        Point a_;
        Point b_;
        Point c_;
    };

    double triangleArea(const Point& a, const Point& b, const Point& c);

} // namespace geox::dim2
