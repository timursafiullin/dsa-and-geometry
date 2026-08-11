#include "geox/dim3/primitives/triangle.h"

#include <cmath>


namespace geox::dim3
{
    // MARK: Triangle
    Triangle::Triangle(const Point& a, const Point& b, const Point& c) : a_(a), b_(b), c_(c) {}

    const Point& Triangle::a() const { return a_; }
    const Point& Triangle::b() const { return b_; }
    const Point& Triangle::c() const { return c_; }

    Vector Triangle::edgeAB() const { return b_ - a_; }
    Vector Triangle::edgeAC() const { return c_ - a_; }

    Vector Triangle::areaNormal() const { return edgeAB().cross(edgeAC()); }

    double Triangle::area() const { return areaNormal().norm() / 2.0; }

    Vector Triangle::normal() const { return areaNormal().normalized(); }

    bool Triangle::isDegenerate(double relativeEpsilon) const
    {
        const Vector ab = edgeAB();
        const Vector ac = edgeAC();
        return areaNormal().norm() <= relativeEpsilon * ab.norm() * ac.norm();
    }

    double triangleArea(
        const Point& a,
        const Point& b,
        const Point& c
    )
    {
        return (b - a).cross(c - a).norm() / 2.0;
    }

} // namespace geox::dim3
