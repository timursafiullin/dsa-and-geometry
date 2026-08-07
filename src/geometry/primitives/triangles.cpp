#include "geometry/primitives/triangles.h"

#include <cmath>

namespace dsa::geometry
{

    // MARK: Triangle2
    Triangle2::Triangle2(const Point2& a, const Point2& b, const Point2& c) : a_(a), b_(b), c_(c) {}

    const Point2& Triangle2::a() const { return a_; }
    const Point2& Triangle2::b() const { return b_; }
    const Point2& Triangle2::c() const { return c_; }

    vector2d Triangle2::edgeAB() const { return b_ - a_; }
    vector2d Triangle2::edgeAC() const { return c_ - a_; }

    double Triangle2::signedDoubleArea() const { return edgeAB().cross(edgeAC()); }

    double Triangle2::area() const { return std::abs(signedDoubleArea()) / 2.0; }

    bool Triangle2::isDegenerate(double relativeEpsilon) const
    {
        const vector2d ab = edgeAB();
        const vector2d ac = edgeAC();
        return std::abs(ab.cross(ac)) <= relativeEpsilon * ab.norm() * ac.norm();
    }

    double triangleArea(
        const Point2& a,
        const Point2& b,
        const Point2& c
    )
    {
        return std::abs((b - a).cross(c - a)) / 2.0;
    }

    // MARK: Triangle3
    Triangle3::Triangle3(const Point3& a, const Point3& b, const Point3& c) : a_(a), b_(b), c_(c) {}

    const Point3& Triangle3::a() const { return a_; }
    const Point3& Triangle3::b() const { return b_; }
    const Point3& Triangle3::c() const { return c_; }

    vector3d Triangle3::edgeAB() const { return b_ - a_; }
    vector3d Triangle3::edgeAC() const { return c_ - a_; }

    vector3d Triangle3::areaNormal() const { return edgeAB().cross(edgeAC()); }

    double Triangle3::area() const { return areaNormal().norm() / 2.0; }

    vector3d Triangle3::normal() const { return areaNormal().normalized(); }

    bool Triangle3::isDegenerate(double relativeEpsilon) const
    {
        const vector3d ab = edgeAB();
        const vector3d ac = edgeAC();
        return areaNormal().norm() <= relativeEpsilon * ab.norm() * ac.norm();
    }

    double triangleArea(
        const Point3& a,
        const Point3& b,
        const Point3& c
    )
    {
        return (b - a).cross(c - a).norm() / 2.0;
    }

} // namespace dsa::geometry
