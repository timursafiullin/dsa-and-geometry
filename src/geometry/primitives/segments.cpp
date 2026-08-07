#include "geometry/primitives/segments.h"

#include <stdexcept>

namespace dsa::geometry
{

    // MARK: Segment2
    Segment2::Segment2(const Point2& start, const Point2& end) : start_(start), end_(end) {}

    const Point2& Segment2::start() const { return start_; }
    const Point2& Segment2::end() const { return end_; }

    vector2d Segment2::direction() const { return vector2d(end_.x() - start_.x(), end_.y() - start_.y()); }

    double Segment2::length() const { return direction().norm(); }

    Point2 Segment2::pointAt(double t) const
    {
        if (t < 0 || t > 1)
            throw std::invalid_argument("Parameter \'t\' should be in [0, 1].");
        Point2 a = start(), b = end();
        return Point2(a.x() + (b.x() - a.x()) * t, a.y() + (b.y() - a.y()) * t);
    }

    // MARK: Segment3
    Segment3::Segment3(const Point3& start, const Point3& end) : start_(start), end_(end) {}

    const Point3& Segment3::start() const { return start_; }
    const Point3& Segment3::end() const { return end_; }

    vector3d Segment3::direction() const { return vector3d(end_.x() - start_.x(), end_.y() - start_.y(), end_.z() - start_.z()); }

    double Segment3::length() const { return direction().norm(); }

    Point3 Segment3::pointAt(double t) const
    {
        if (t < 0 || t > 1)
            throw std::invalid_argument("Parameter \'t\' should be in [0, 1].");
        Point3 a = start(), b = end();
        return Point3(a.x() + (b.x() - a.x()) * t, a.y() + (b.y() - a.y()) * t, a.z() + (b.z() - a.z()) * t);
    }

} // namespace dsa::geometry