#include "geox/dim3/primitives/segment.h"

#include <stdexcept>


namespace geox::dim3
{
    // MARK: Segment
    Segment::Segment(const Point& start, const Point& end) : start_(start), end_(end) {}

    const Point& Segment::start() const { return start_; }
    const Point& Segment::end() const { return end_; }

    Vector Segment::direction() const { return Vector(end_.x() - start_.x(), end_.y() - start_.y(), end_.z() - start_.z()); }

    double Segment::length() const { return direction().norm(); }

    Point Segment::pointAt(double t) const
    {
        if (t < 0 || t > 1)
            throw std::invalid_argument("Parameter \'t\' should be in [0, 1].");
        Point a = start(), b = end();
        return Point(a.x() + (b.x() - a.x()) * t, a.y() + (b.y() - a.y()) * t, a.z() + (b.z() - a.z()) * t);
    }

} // namespace geox::dim3
