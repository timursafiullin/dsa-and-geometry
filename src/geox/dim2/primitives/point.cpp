#include "geox/dim2/primitives/point.h"
#include <cmath>
#include <ostream>
#include <stdexcept>


namespace geox::dim2
{
    // MARK: Point
    Point::Point() : data{0.0, 0.0} {}

    Point::Point(double x, double y) : data{x, y} {}

    Point::Point(const Point &other) : data{other.data[0], other.data[1]} {}

    Point &Point::operator=(const Point &other)
    {
        if (this != &other)
        {
            data[0] = other.data[0];
            data[1] = other.data[1];
        }
        return *this;
    }

    double &Point::x() { return data[0]; }
    double &Point::y() { return data[1]; }

    const double &Point::x() const { return data[0]; }
    const double &Point::y() const { return data[1]; }

    double &Point::operator[](int index)
    {
        if (index < 0 || index > 1)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const double &Point::operator[](int index) const
    {
        if (index < 0 || index > 1)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    Point Point::operator+(const Vector &vec) const { return Point(data[0] + vec.x(), data[1] + vec.y()); }

    Point Point::operator-(const Vector &vec) const { return Point(data[0] - vec.x(), data[1] - vec.y()); }

    Vector Point::operator-(const Point &other) const
    {
        return Vector(data[0] - other.data[0], data[1] - other.data[1]);
    }

    Point &Point::operator+=(const Vector &vec)
    {
        data[0] += vec.x();
        data[1] += vec.y();
        return *this;
    }

    Point &Point::operator-=(const Vector &vec)
    {
        data[0] -= vec.x();
        data[1] -= vec.y();
        return *this;
    }

    double Point::squaredDistance(const Point &other) const { return (*this - other).squaredNorm(); }

    double Point::distance(const Point &other) const { return std::sqrt(squaredDistance(other)); }

    bool Point::operator==(const Point &other) const { return data[0] == other.data[0] && data[1] == other.data[1]; }

    bool Point::operator!=(const Point &other) const { return !(*this == other); }

    std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        os << "(" << point.data[0] << ", " << point.data[1] << ")";
        return os;
    }

    double distance(const Point& a, const Point& b) { return a.distance(b); }

} // namespace geox::dim2
