#include "geox/primitives/points.h"
#include <cmath>
#include <ostream>
#include <stdexcept>

namespace geox
{

    // MARK: Point2
    Point2::Point2() : data{0.0, 0.0} {}

    Point2::Point2(double x, double y) : data{x, y} {}

    Point2::Point2(const Point2 &other) : data{other.data[0], other.data[1]} {}

    Point2 &Point2::operator=(const Point2 &other)
    {
        if (this != &other)
        {
            data[0] = other.data[0];
            data[1] = other.data[1];
        }
        return *this;
    }

    double &Point2::x() { return data[0]; }
    double &Point2::y() { return data[1]; }

    const double &Point2::x() const { return data[0]; }
    const double &Point2::y() const { return data[1]; }

    double &Point2::operator[](int index)
    {
        if (index < 0 || index > 1)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const double &Point2::operator[](int index) const
    {
        if (index < 0 || index > 1)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    Point2 Point2::operator+(const vector2d &vec) const { return Point2(data[0] + vec.x(), data[1] + vec.y()); }

    Point2 Point2::operator-(const vector2d &vec) const { return Point2(data[0] - vec.x(), data[1] - vec.y()); }

    vector2d Point2::operator-(const Point2 &other) const
    {
        return vector2d(data[0] - other.data[0], data[1] - other.data[1]);
    }

    Point2 &Point2::operator+=(const vector2d &vec)
    {
        data[0] += vec.x();
        data[1] += vec.y();
        return *this;
    }

    Point2 &Point2::operator-=(const vector2d &vec)
    {
        data[0] -= vec.x();
        data[1] -= vec.y();
        return *this;
    }

    double Point2::squaredDistance(const Point2 &other) const { return (*this - other).squaredNorm(); }

    double Point2::distance(const Point2 &other) const { return std::sqrt(squaredDistance(other)); }

    bool Point2::operator==(const Point2 &other) const { return data[0] == other.data[0] && data[1] == other.data[1]; }

    bool Point2::operator!=(const Point2 &other) const { return !(*this == other); }

    std::ostream &operator<<(std::ostream &os, const Point2 &point)
    {
        os << "(" << point.data[0] << ", " << point.data[1] << ")";
        return os;
    }

    double distance(const Point2& a, const Point2& b) { return a.distance(b); }

    // MARK: Point3
    Point3::Point3() : data{0.0, 0.0, 0.0} {}

    Point3::Point3(double x, double y, double z) : data{x, y, z} {}

    Point3::Point3(const Point3 &other) : data{other.data[0], other.data[1], other.data[2]} {}

    Point3 &Point3::operator=(const Point3 &other)
    {
        if (this != &other)
        {
            data[0] = other.data[0];
            data[1] = other.data[1];
            data[2] = other.data[2];
        }
        return *this;
    }

    double &Point3::x() { return data[0]; }
    double &Point3::y() { return data[1]; }
    double &Point3::z() { return data[2]; }

    const double &Point3::x() const { return data[0]; }
    const double &Point3::y() const { return data[1]; }
    const double &Point3::z() const { return data[2]; }

    double &Point3::operator[](int index)
    {
        if (index < 0 || index > 2)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const double &Point3::operator[](int index) const
    {
        if (index < 0 || index > 2)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    Point3 Point3::operator+(const vector3d &vec) const
    {
        return Point3(data[0] + vec.x(), data[1] + vec.y(), data[2] + vec.z());
    }

    Point3 Point3::operator-(const vector3d &vec) const
    {
        return Point3(data[0] - vec.x(), data[1] - vec.y(), data[2] - vec.z());
    }

    vector3d Point3::operator-(const Point3 &other) const
    {
        return vector3d(data[0] - other.data[0], data[1] - other.data[1], data[2] - other.data[2]);
    }

    Point3 &Point3::operator+=(const vector3d &vec)
    {
        data[0] += vec.x();
        data[1] += vec.y();
        data[2] += vec.z();
        return *this;
    }

    Point3 &Point3::operator-=(const vector3d &vec)
    {
        data[0] -= vec.x();
        data[1] -= vec.y();
        data[2] -= vec.z();
        return *this;
    }

    double Point3::squaredDistance(const Point3 &other) const { return (*this - other).squaredNorm(); }

    double Point3::distance(const Point3 &other) const { return std::sqrt(squaredDistance(other)); }

    bool Point3::operator==(const Point3 &other) const
    {
        return data[0] == other.data[0] && data[1] == other.data[1] && data[2] == other.data[2];
    }

    bool Point3::operator!=(const Point3 &other) const { return !(*this == other); }

    std::ostream &operator<<(std::ostream &os, const Point3 &point)
    {
        os << "(" << point.data[0] << ", " << point.data[1] << ", " << point.data[2] << ")";
        return os;
    }

    double distance(const Point3& a, const Point3& b) { return a.distance(b); }

} // namespace geox
