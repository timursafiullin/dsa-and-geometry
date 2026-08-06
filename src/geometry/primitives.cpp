#include "geometry/primitives.h"
#include <cmath>
#include <ostream>
#include <stdexcept>

namespace dsa::geometry
{
    // Constructors
    vector2d::vector2d() : data{0.0, 0.0} {}

    vector2d::vector2d(double x, double y) : data{x, y} {}

    vector2d::vector2d(const vector2d &other) : data{other.data[0], other.data[1]} {}

    vector2d &vector2d::operator=(const vector2d &other)
    {
        if (this != &other)
        {
            data[0] = other.data[0];
            data[1] = other.data[1];
        }
        return *this;
    }

    // Component access
    double &vector2d::x() { return data[0]; }
    double &vector2d::y() { return data[1]; }

    const double &vector2d::x() const { return data[0]; }
    const double &vector2d::y() const { return data[1]; }

    // Indexed access
    double &vector2d::operator[](int index)
    {
        if (index < 0 || index > 1)
        {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const double &vector2d::operator[](int index) const
    {
        if (index < 0 || index > 1)
        {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Arithmetic operations
    vector2d vector2d::operator+(const vector2d &other) const
    {
        return vector2d(data[0] + other.data[0],
                        data[1] + other.data[1]);
    }

    vector2d vector2d::operator-(const vector2d &other) const
    {
        return vector2d(data[0] - other.data[0],
                        data[1] - other.data[1]);
    }

    vector2d vector2d::operator*(double scalar) const
    {
        return vector2d(data[0] * scalar, data[1] * scalar);
    }

    vector2d vector2d::operator/(double scalar) const
    {
        if (scalar == 0.0)
        {
            throw std::invalid_argument("Division by zero");
        }
        return vector2d(data[0] / scalar, data[1] / scalar);
    }

    // Compound assignment operators
    vector2d &vector2d::operator+=(const vector2d &other)
    {
        data[0] += other.data[0];
        data[1] += other.data[1];
        return *this;
    }

    vector2d &vector2d::operator-=(const vector2d &other)
    {
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        return *this;
    }

    vector2d &vector2d::operator*=(double scalar)
    {
        data[0] *= scalar;
        data[1] *= scalar;
        return *this;
    }

    vector2d &vector2d::operator/=(double scalar)
    {
        if (scalar == 0.0)
        {
            throw std::invalid_argument("Division by zero");
        }
        data[0] /= scalar;
        data[1] /= scalar;
        return *this;
    }

    // Unary operators
    vector2d vector2d::operator-() const
    {
        return vector2d(-data[0], -data[1]);
    }

    // Mathematical operations
    double vector2d::dot(const vector2d &other) const
    {
        return data[0] * other.data[0] + data[1] * other.data[1];
    }

    double vector2d::cross(const vector2d &other) const
    {
        return data[0] * other.data[1] - data[1] * other.data[0];
    }

    double vector2d::norm() const
    {
        return std::sqrt(squaredNorm());
    }

    double vector2d::squaredNorm() const
    {
        return data[0] * data[0] + data[1] * data[1];
    }

    vector2d vector2d::normalized() const
    {
        double n = norm();
        if (n == 0.0)
        {
            throw std::invalid_argument("Cannot normalize zero vector");
        }
        return *this / n;
    }

    vector2d &vector2d::normalize()
    {
        double n = norm();
        if (n == 0.0)
        {
            throw std::invalid_argument("Cannot normalize zero vector");
        }
        *this /= n;
        return *this;
    }

    // Utility functions
    bool vector2d::isZero(double epsilon) const
    {
        return squaredNorm() < epsilon;
    }

    bool vector2d::operator==(const vector2d &other) const
    {
        return data[0] == other.data[0] && data[1] == other.data[1];
    }

    bool vector2d::operator!=(const vector2d &other) const
    {
        return !(*this == other);
    }

    // Stream output
    std::ostream &operator<<(std::ostream &os, const vector2d &vec)
    {
        os << "(" << vec.data[0] << ", " << vec.data[1] << ")";
        return os;
    }

    // Scalar multiplication
    vector2d operator*(double scalar, const vector2d &vec)
    {
        return vec * scalar;
    }

    // Constructors
    vector3d::vector3d() : data{0.0, 0.0, 0.0} {}

    vector3d::vector3d(double x, double y, double z) : data{x, y, z} {}

    vector3d::vector3d(const vector3d &other) : data{other.data[0], other.data[1], other.data[2]} {}

    vector3d &vector3d::operator=(const vector3d &other)
    {
        if (this != &other)
        {
            data[0] = other.data[0];
            data[1] = other.data[1];
            data[2] = other.data[2];
        }
        return *this;
    }

    // Component access
    double &vector3d::x() { return data[0]; }
    double &vector3d::y() { return data[1]; }
    double &vector3d::z() { return data[2]; }

    const double &vector3d::x() const { return data[0]; }
    const double &vector3d::y() const { return data[1]; }
    const double &vector3d::z() const { return data[2]; }

    // Indexed access
    double &vector3d::operator[](int index)
    {
        if (index < 0 || index > 2)
        {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const double &vector3d::operator[](int index) const
    {
        if (index < 0 || index > 2)
        {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Arithmetic operations
    vector3d vector3d::operator+(const vector3d &other) const
    {
        return vector3d(data[0] + other.data[0],
                        data[1] + other.data[1],
                        data[2] + other.data[2]);
    }

    vector3d vector3d::operator-(const vector3d &other) const
    {
        return vector3d(data[0] - other.data[0],
                        data[1] - other.data[1],
                        data[2] - other.data[2]);
    }

    vector3d vector3d::operator*(double scalar) const
    {
        return vector3d(data[0] * scalar,
                        data[1] * scalar,
                        data[2] * scalar);
    }

    vector3d vector3d::operator/(double scalar) const
    {
        if (scalar == 0.0)
        {
            throw std::invalid_argument("Division by zero");
        }
        return vector3d(data[0] / scalar,
                        data[1] / scalar,
                        data[2] / scalar);
    }

    // Compound assignment operators
    vector3d &vector3d::operator+=(const vector3d &other)
    {
        data[0] += other.data[0];
        data[1] += other.data[1];
        data[2] += other.data[2];
        return *this;
    }

    vector3d &vector3d::operator-=(const vector3d &other)
    {
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        data[2] -= other.data[2];
        return *this;
    }

    vector3d &vector3d::operator*=(double scalar)
    {
        data[0] *= scalar;
        data[1] *= scalar;
        data[2] *= scalar;
        return *this;
    }

    vector3d &vector3d::operator/=(double scalar)
    {
        if (scalar == 0.0)
        {
            throw std::invalid_argument("Division by zero");
        }
        data[0] /= scalar;
        data[1] /= scalar;
        data[2] /= scalar;
        return *this;
    }

    // Unary operators
    vector3d vector3d::operator-() const
    {
        return vector3d(-data[0], -data[1], -data[2]);
    }

    // Mathematical operations
    double vector3d::dot(const vector3d &other) const
    {
        return data[0] * other.data[0] +
               data[1] * other.data[1] +
               data[2] * other.data[2];
    }

    vector3d vector3d::cross(const vector3d &other) const
    {
        return vector3d(
            data[1] * other.data[2] - data[2] * other.data[1],
            data[2] * other.data[0] - data[0] * other.data[2],
            data[0] * other.data[1] - data[1] * other.data[0]);
    }

    double vector3d::norm() const
    {
        return std::sqrt(data[0] * data[0] +
                         data[1] * data[1] +
                         data[2] * data[2]);
    }

    double vector3d::squaredNorm() const
    {
        return data[0] * data[0] +
               data[1] * data[1] +
               data[2] * data[2];
    }

    vector3d vector3d::normalized() const
    {
        double n = norm();
        if (n == 0.0)
        {
            throw std::invalid_argument("Cannot normalize zero vector");
        }
        return *this / n;
    }

    vector3d &vector3d::normalize()
    {
        double n = norm();
        if (n == 0.0)
        {
            throw std::invalid_argument("Cannot normalize zero vector");
        }
        *this /= n;
        return *this;
    }

    // Utility functions
    bool vector3d::isZero(double epsilon) const
    {
        return squaredNorm() < epsilon;
    }

    bool vector3d::operator==(const vector3d &other) const
    {
        return data[0] == other.data[0] &&
               data[1] == other.data[1] &&
               data[2] == other.data[2];
    }

    bool vector3d::operator!=(const vector3d &other) const
    {
        return !(*this == other);
    }

    // Stream output
    std::ostream &operator<<(std::ostream &os, const vector3d &vec)
    {
        os << "(" << vec.data[0] << ", " << vec.data[1] << ", " << vec.data[2] << ")";
        return os;
    }

    // Scalar multiplication
    vector3d operator*(double scalar, const vector3d &vec)
    {
        return vec * scalar;
    }

    // Constructors
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

    // Component access
    double &Point2::x() { return data[0]; }
    double &Point2::y() { return data[1]; }
    const double &Point2::x() const { return data[0]; }
    const double &Point2::y() const { return data[1]; }

    // Indexed access
    double &Point2::operator[](int index)
    {
        if (index < 0 || index > 1) throw std::out_of_range("Index out of range");
        return data[index];
    }
    const double &Point2::operator[](int index) const
    {
        if (index < 0 || index > 1) throw std::out_of_range("Index out of range");
        return data[index];
    }

    Point2 Point2::operator+(const vector2d &vec) const { return Point2(data[0] + vec.x(), data[1] + vec.y()); }
    Point2 Point2::operator-(const vector2d &vec) const { return Point2(data[0] - vec.x(), data[1] - vec.y()); }
    vector2d Point2::operator-(const Point2 &other) const { return vector2d(data[0] - other.data[0], data[1] - other.data[1]); }
    Point2 &Point2::operator+=(const vector2d &vec) { data[0] += vec.x(); data[1] += vec.y(); return *this; }
    Point2 &Point2::operator-=(const vector2d &vec) { data[0] -= vec.x(); data[1] -= vec.y(); return *this; }
    double Point2::squaredDistance(const Point2 &other) const { return (*this - other).squaredNorm(); }
    double Point2::distance(const Point2 &other) const { return std::sqrt(squaredDistance(other)); }
    bool Point2::operator==(const Point2 &other) const { return data[0] == other.data[0] && data[1] == other.data[1]; }
    bool Point2::operator!=(const Point2 &other) const { return !(*this == other); }
    std::ostream &operator<<(std::ostream &os, const Point2 &point) { os << "(" << point.data[0] << ", " << point.data[1] << ")"; return os; }

    // Constructors
    Point3::Point3() : data{0.0, 0.0, 0.0} {}
    Point3::Point3(double x, double y, double z) : data{x, y, z} {}
    Point3::Point3(const Point3 &other) : data{other.data[0], other.data[1], other.data[2]} {}
    Point3 &Point3::operator=(const Point3 &other)
    {
        if (this != &other) { data[0] = other.data[0]; data[1] = other.data[1]; data[2] = other.data[2]; }
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
        if (index < 0 || index > 2) throw std::out_of_range("Index out of range");
        return data[index];
    }
    const double &Point3::operator[](int index) const
    {
        if (index < 0 || index > 2) throw std::out_of_range("Index out of range");
        return data[index];
    }
    Point3 Point3::operator+(const vector3d &vec) const { return Point3(data[0] + vec.x(), data[1] + vec.y(), data[2] + vec.z()); }
    Point3 Point3::operator-(const vector3d &vec) const { return Point3(data[0] - vec.x(), data[1] - vec.y(), data[2] - vec.z()); }
    vector3d Point3::operator-(const Point3 &other) const { return vector3d(data[0] - other.data[0], data[1] - other.data[1], data[2] - other.data[2]); }
    Point3 &Point3::operator+=(const vector3d &vec) { data[0] += vec.x(); data[1] += vec.y(); data[2] += vec.z(); return *this; }
    Point3 &Point3::operator-=(const vector3d &vec) { data[0] -= vec.x(); data[1] -= vec.y(); data[2] -= vec.z(); return *this; }
    double Point3::squaredDistance(const Point3 &other) const { return (*this - other).squaredNorm(); }
    double Point3::distance(const Point3 &other) const { return std::sqrt(squaredDistance(other)); }
    bool Point3::operator==(const Point3 &other) const { return data[0] == other.data[0] && data[1] == other.data[1] && data[2] == other.data[2]; }
    bool Point3::operator!=(const Point3 &other) const { return !(*this == other); }
    std::ostream &operator<<(std::ostream &os, const Point3 &point) { os << "(" << point.data[0] << ", " << point.data[1] << ", " << point.data[2] << ")"; return os; }
}
