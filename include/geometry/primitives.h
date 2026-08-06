#pragma once

#include <iosfwd>

namespace dsa::geometry
{
    class vector3d
    {
    private:
        double data[3];

    public:
        // Constructors
        vector3d();
        vector3d(double x, double y, double z);
        vector3d(const vector3d &other);

        vector3d &operator=(const vector3d &other);

        // Component access
        double &x();
        double &y();
        double &z();

        const double &x() const;
        const double &y() const;
        const double &z() const;

        // Indexed access
        double &operator[](int index);
        const double &operator[](int index) const;

        // Arithmetic operations
        vector3d operator+(const vector3d &other) const;
        vector3d operator-(const vector3d &other) const;
        vector3d operator*(double scalar) const;
        vector3d operator/(double scalar) const;

        vector3d &operator+=(const vector3d &other);
        vector3d &operator-=(const vector3d &other);
        vector3d &operator*=(double scalar);
        vector3d &operator/=(double scalar);

        vector3d operator-() const;

        // Vector operations
        double dot(const vector3d &other) const;
        vector3d cross(const vector3d &other) const;
        double norm() const;
        double squaredNorm() const;
        vector3d normalized() const;
        vector3d &normalize();

        // Utility functions
        bool isZero(double epsilon = 1e-10) const;
        bool operator==(const vector3d &other) const;
        bool operator!=(const vector3d &other) const;

        // Stream output
        friend std::ostream &operator<<(std::ostream &os, const vector3d &vec);
    };

    // Scalar multiplication
    vector3d operator*(double scalar, const vector3d &vec);
}
