#include "geox/dim2/primitives/segment.h"

#include <stdexcept>


namespace geox::dim2
{
    // MARK: Segment
    Segment::Segment(const Point& start, const Point& end) : start_(start), end_(end) {}

    const Point& Segment::start() const { return start_; }
    const Point& Segment::end() const { return end_; }

    Vector Segment::direction() const { return Vector(end_.x() - start_.x(), end_.y() - start_.y()); }

    double Segment::length() const { return direction().norm(); }

    Point Segment::pointAt(double t) const
    {
        if (t < 0 || t > 1)
            throw std::invalid_argument("Parameter \'t\' should be in [0, 1].");
        Point a = start(), b = end();
        return Point(a.x() + (b.x() - a.x()) * t, a.y() + (b.y() - a.y()) * t);
    }

} // namespace geox::dim2
