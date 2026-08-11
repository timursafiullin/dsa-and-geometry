#pragma once

#include "geox/core/constants.h"

#include <iosfwd>

namespace geox::dim2
{

    class Vector
    {
    private:
        double data[2];
    public:
        Vector();
        Vector(double x, double y);
        Vector(const Vector &other);

        Vector &operator=(const Vector &other);

        double &x();
        double &y();
        const double &x() const;
        const double &y() const;

        double &operator[](int index);
        const double &operator[](int index) const;

        Vector operator+(const Vector &other) const;
        Vector operator-(const Vector &other) const;
        Vector operator*(double scalar) const;
        Vector operator/(double scalar) const;

        Vector &operator+=(const Vector &other);
        Vector &operator-=(const Vector &other);
        Vector &operator*=(double scalar);
        Vector &operator/=(double scalar);
        Vector operator-() const;

        double dot(const Vector &other) const;
        double cross(const Vector &other) const;

        double norm() const;
        double squaredNorm() const;
        Vector normalized() const;
        Vector &normalize();

        bool isZero(double linearEpsilon = core::kLinearEpsilon) const;
        bool operator==(const Vector &other) const;
        bool operator!=(const Vector &other) const;

        friend std::ostream &operator<<(std::ostream &os, const Vector &vec);
    };

    Vector operator*(double scalar, const Vector &vec);

} // namespace geox::dim2
