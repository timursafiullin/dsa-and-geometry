#include "geox/dim2/primitives/triangle.h"

#include <cmath>


namespace geox::dim2
{
    // MARK: Triangle
    Triangle::Triangle(const Point& a, const Point& b, const Point& c) : a_(a), b_(b), c_(c) {}

    const Point& Triangle::a() const { return a_; }
    const Point& Triangle::b() const { return b_; }
    const Point& Triangle::c() const { return c_; }

    Vector Triangle::edgeAB() const { return b_ - a_; }
    Vector Triangle::edgeAC() const { return c_ - a_; }

    double Triangle::signedDoubleArea() const { return edgeAB().cross(edgeAC()); }

    double Triangle::area() const { return std::abs(signedDoubleArea()) / 2.0; }

    bool Triangle::isDegenerate(double relativeEpsilon) const
    {
        const Vector ab = edgeAB();
        const Vector ac = edgeAC();
        return std::abs(ab.cross(ac)) <= relativeEpsilon * ab.norm() * ac.norm();
    }

    double triangleArea(
        const Point& a,
        const Point& b,
        const Point& c
    )
    {
        return std::abs((b - a).cross(c - a)) / 2.0;
    }

} // namespace geox::dim2
