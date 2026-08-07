#pragma once

#include "geometry/constants.h"

#include <iosfwd>

namespace dsa::geometry
{

    class vector2d
    {
    private:
        double data[2];
    public:
        vector2d();
        vector2d(double x, double y);
        vector2d(const vector2d &other);

        vector2d &operator=(const vector2d &other);

        double &x();
        double &y();
        const double &x() const;
        const double &y() const;

        double &operator[](int index);
        const double &operator[](int index) const;

        vector2d operator+(const vector2d &other) const;
        vector2d operator-(const vector2d &other) const;
        vector2d operator*(double scalar) const;
        vector2d operator/(double scalar) const;

        vector2d &operator+=(const vector2d &other);
        vector2d &operator-=(const vector2d &other);
        vector2d &operator*=(double scalar);
        vector2d &operator/=(double scalar);
        vector2d operator-() const;

        double dot(const vector2d &other) const;
        double cross(const vector2d &other) const;

        double norm() const;
        double squaredNorm() const;
        vector2d normalized() const;
        vector2d &normalize();

        bool isZero(double linearEpsilon = kLinearEpsilon) const;
        bool operator==(const vector2d &other) const;
        bool operator!=(const vector2d &other) const;

        friend std::ostream &operator<<(std::ostream &os, const vector2d &vec);
    };

    vector2d operator*(double scalar, const vector2d &vec);

    class vector3d
    {
    private:
        double data[3];
    public:
        vector3d();
        vector3d(double x, double y, double z);
        vector3d(const vector3d &other);
        
        vector3d &operator=(const vector3d &other);

        double &x(); double &y(); double &z();
        const double &x() const;
        const double &y() const;
        const double &z() const;

        double &operator[](int index);
        const double &operator[](int index) const;

        vector3d operator+(const vector3d &other) const;
        vector3d operator-(const vector3d &other) const;
        vector3d operator*(double scalar) const;
        vector3d operator/(double scalar) const;

        vector3d &operator+=(const vector3d &other);
        vector3d &operator-=(const vector3d &other);
        vector3d &operator*=(double scalar);
        vector3d &operator/=(double scalar);
        vector3d operator-() const;

        double dot(const vector3d &other) const;
        vector3d cross(const vector3d &other) const;

        double norm() const;
        double squaredNorm() const;
        vector3d normalized() const;
        vector3d &normalize();

        bool isZero(double linearEpsilon = kLinearEpsilon) const;
        bool operator==(const vector3d &other) const;
        bool operator!=(const vector3d &other) const;

        friend std::ostream &operator<<(std::ostream &os, const vector3d &vec);
    };

    vector3d operator*(double scalar, const vector3d &vec);

} // namespace dsa::geometry
