#include "geox/dim3/primitives/vector.h"
#include <cmath>
#include <ostream>
#include <stdexcept>


namespace geox::dim3
{
    // MARK: Vector
    Vector::Vector() : data{0.0, 0.0, 0.0} {}

    Vector::Vector(double x, double y, double z) : data{x, y, z} {}

    Vector::Vector(const Vector &other) : data{other.data[0], other.data[1], other.data[2]} {}

    Vector &Vector::operator=(const Vector &other)
    {
        if (this != &other)
        {
            data[0] = other.data[0];
            data[1] = other.data[1];
            data[2] = other.data[2];
        }
        return *this;
    }

    double &Vector::x() { return data[0]; }
    double &Vector::y() { return data[1]; }
    double &Vector::z() { return data[2]; }

    const double &Vector::x() const { return data[0]; }
    const double &Vector::y() const { return data[1]; }
    const double &Vector::z() const { return data[2]; }

    double &Vector::operator[](int index)
    {
        if (index < 0 || index > 2)
        {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const double &Vector::operator[](int index) const
    {
        if (index < 0 || index > 2)
        {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    Vector Vector::operator+(const Vector &other) const
    {
        return Vector(data[0] + other.data[0], data[1] + other.data[1], data[2] + other.data[2]);
    }

    Vector Vector::operator-(const Vector &other) const
    {
        return Vector(data[0] - other.data[0], data[1] - other.data[1], data[2] - other.data[2]);
    }

    Vector Vector::operator*(double scalar) const
    {
        return Vector(data[0] * scalar, data[1] * scalar, data[2] * scalar);
    }

    Vector Vector::operator/(double scalar) const
    {
        if (scalar == 0.0)
        {
            throw std::invalid_argument("Division by zero");
        }
        return Vector(data[0] / scalar, data[1] / scalar, data[2] / scalar);
    }

    Vector &Vector::operator+=(const Vector &other)
    {
        data[0] += other.data[0];
        data[1] += other.data[1];
        data[2] += other.data[2];
        return *this;
    }

    Vector &Vector::operator-=(const Vector &other)
    {
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        data[2] -= other.data[2];
        return *this;
    }

    Vector &Vector::operator*=(double scalar)
    {
        data[0] *= scalar;
        data[1] *= scalar;
        data[2] *= scalar;
        return *this;
    }

    Vector &Vector::operator/=(double scalar)
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

    Vector Vector::operator-() const { return Vector(-data[0], -data[1], -data[2]); }

    double Vector::dot(const Vector &other) const
    {
        return data[0] * other.data[0] + data[1] * other.data[1] + data[2] * other.data[2];
    }

    Vector Vector::cross(const Vector &other) const
    {
        return Vector(data[1] * other.data[2] - data[2] * other.data[1],
                        data[2] * other.data[0] - data[0] * other.data[2],
                        data[0] * other.data[1] - data[1] * other.data[0]);
    }

    double Vector::norm() const { return std::sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]); }

    double Vector::squaredNorm() const { return data[0] * data[0] + data[1] * data[1] + data[2] * data[2]; }

    Vector Vector::normalized() const
    {
        double n = norm();
        if (n == 0.0)
        {
            throw std::invalid_argument("Cannot normalize zero vector");
        }
        return *this / n;
    }

    Vector &Vector::normalize()
    {
        double n = norm();
        if (n == 0.0)
        {
            throw std::invalid_argument("Cannot normalize zero vector");
        }
        *this /= n;
        return *this;
    }

    bool Vector::isZero(double linearEpsilon) const { return norm() < linearEpsilon; }

    bool Vector::operator==(const Vector &other) const
    {
        return data[0] == other.data[0] && data[1] == other.data[1] && data[2] == other.data[2];
    }

    bool Vector::operator!=(const Vector &other) const { return !(*this == other); }

    std::ostream &operator<<(std::ostream &os, const Vector &vec)
    {
        os << "(" << vec.data[0] << ", " << vec.data[1] << ", " << vec.data[2] << ")";
        return os;
    }

    Vector operator*(double scalar, const Vector &vec) { return vec * scalar; }

} // namespace geox::dim3
