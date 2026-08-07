#include "geometry/primitives/vectors.h"
#include <cmath>
#include <ostream>
#include <stdexcept>

namespace dsa::geometry
{
    // MARK: vector2d
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

    double &vector2d::x() { return data[0]; }
    double &vector2d::y() { return data[1]; }

    const double &vector2d::x() const { return data[0]; }
    const double &vector2d::y() const { return data[1]; }

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

    vector2d vector2d::operator+(const vector2d &other) const
    {
        return vector2d(data[0] + other.data[0], data[1] + other.data[1]);
    }

    vector2d vector2d::operator-(const vector2d &other) const
    {
        return vector2d(data[0] - other.data[0], data[1] - other.data[1]);
    }

    vector2d vector2d::operator*(double scalar) const { return vector2d(data[0] * scalar, data[1] * scalar); }

    vector2d vector2d::operator/(double scalar) const
    {
        if (scalar == 0.0)
        {
            throw std::invalid_argument("Division by zero");
        }
        return vector2d(data[0] / scalar, data[1] / scalar);
    }

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

    vector2d vector2d::operator-() const { return vector2d(-data[0], -data[1]); }

    double vector2d::dot(const vector2d &other) const { return data[0] * other.data[0] + data[1] * other.data[1]; }

    double vector2d::cross(const vector2d &other) const { return data[0] * other.data[1] - data[1] * other.data[0]; }

    double vector2d::norm() const { return std::sqrt(squaredNorm()); }

    double vector2d::squaredNorm() const { return data[0] * data[0] + data[1] * data[1]; }

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

    bool vector2d::isZero(double epsilon) const { return norm() < epsilon; }

    bool vector2d::operator==(const vector2d &other) const
    {
        return data[0] == other.data[0] && data[1] == other.data[1];
    }

    bool vector2d::operator!=(const vector2d &other) const { return !(*this == other); }

    std::ostream &operator<<(std::ostream &os, const vector2d &vec)
    {
        os << "(" << vec.data[0] << ", " << vec.data[1] << ")";
        return os;
    }

    vector2d operator*(double scalar, const vector2d &vec) { return vec * scalar; }

    // MARK: vector3d
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

    double &vector3d::x() { return data[0]; }
    double &vector3d::y() { return data[1]; }
    double &vector3d::z() { return data[2]; }

    const double &vector3d::x() const { return data[0]; }
    const double &vector3d::y() const { return data[1]; }
    const double &vector3d::z() const { return data[2]; }

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

    vector3d vector3d::operator+(const vector3d &other) const
    {
        return vector3d(data[0] + other.data[0], data[1] + other.data[1], data[2] + other.data[2]);
    }

    vector3d vector3d::operator-(const vector3d &other) const
    {
        return vector3d(data[0] - other.data[0], data[1] - other.data[1], data[2] - other.data[2]);
    }

    vector3d vector3d::operator*(double scalar) const
    {
        return vector3d(data[0] * scalar, data[1] * scalar, data[2] * scalar);
    }

    vector3d vector3d::operator/(double scalar) const
    {
        if (scalar == 0.0)
        {
            throw std::invalid_argument("Division by zero");
        }
        return vector3d(data[0] / scalar, data[1] / scalar, data[2] / scalar);
    }

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

    vector3d vector3d::operator-() const { return vector3d(-data[0], -data[1], -data[2]); }

    double vector3d::dot(const vector3d &other) const
    {
        return data[0] * other.data[0] + data[1] * other.data[1] + data[2] * other.data[2];
    }

    vector3d vector3d::cross(const vector3d &other) const
    {
        return vector3d(data[1] * other.data[2] - data[2] * other.data[1],
                        data[2] * other.data[0] - data[0] * other.data[2],
                        data[0] * other.data[1] - data[1] * other.data[0]);
    }

    double vector3d::norm() const { return std::sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]); }

    double vector3d::squaredNorm() const { return data[0] * data[0] + data[1] * data[1] + data[2] * data[2]; }

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

    bool vector3d::isZero(double epsilon) const { return norm() < epsilon; }

    bool vector3d::operator==(const vector3d &other) const
    {
        return data[0] == other.data[0] && data[1] == other.data[1] && data[2] == other.data[2];
    }

    bool vector3d::operator!=(const vector3d &other) const { return !(*this == other); }

    std::ostream &operator<<(std::ostream &os, const vector3d &vec)
    {
        os << "(" << vec.data[0] << ", " << vec.data[1] << ", " << vec.data[2] << ")";
        return os;
    }

    vector3d operator*(double scalar, const vector3d &vec) { return vec * scalar; }

} // namespace dsa::geometry
