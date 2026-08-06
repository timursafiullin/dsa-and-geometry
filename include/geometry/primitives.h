#pragma once

#include <iosfwd>

namespace dsa::geometry
{
    class vector2d
    {
    private:
        double data[2];

    public:
        // Constructors
        vector2d();
        vector2d(double x, double y);
        vector2d(const vector2d &other);

        vector2d &operator=(const vector2d &other);

        // Component access
        double &x();
        double &y();

        const double &x() const;
        const double &y() const;

        // Indexed access
        double &operator[](int index);
        const double &operator[](int index) const;

        // Arithmetic operations
        vector2d operator+(const vector2d &other) const;
        vector2d operator-(const vector2d &other) const;
        vector2d operator*(double scalar) const;
        vector2d operator/(double scalar) const;

        vector2d &operator+=(const vector2d &other);
        vector2d &operator-=(const vector2d &other);
        vector2d &operator*=(double scalar);
        vector2d &operator/=(double scalar);

        vector2d operator-() const;

        // Vector operations
        double dot(const vector2d &other) const;
        double cross(const vector2d &other) const;
        double norm() const;
        double squaredNorm() const;
        vector2d normalized() const;
        vector2d &normalize();

        // Utility functions
        bool isZero(double epsilon = 1e-10) const;
        bool operator==(const vector2d &other) const;
        bool operator!=(const vector2d &other) const;

        // Stream output
        friend std::ostream &operator<<(std::ostream &os, const vector2d &vec);
    };

    // Scalar multiplication
    vector2d operator*(double scalar, const vector2d &vec);

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

    class Point2
    {
    private:
        double data[2];

    public:
        // Constructors
        Point2();
        Point2(double x, double y);
        Point2(const Point2 &other);

        Point2 &operator=(const Point2 &other);

        // Component access
        double &x();
        double &y();

        const double &x() const;
        const double &y() const;

        // Indexed access
        double &operator[](int index);
        const double &operator[](int index) const;

        // Point and vector operations
        Point2 operator+(const vector2d &vec) const;
        Point2 operator-(const vector2d &vec) const;
        vector2d operator-(const Point2 &other) const;

        Point2 &operator+=(const vector2d &vec);
        Point2 &operator-=(const vector2d &vec);

        double squaredDistance(const Point2 &other) const;
        double distance(const Point2 &other) const;

        // Utility functions
        bool operator==(const Point2 &other) const;
        bool operator!=(const Point2 &other) const;

        // Stream output
        friend std::ostream &operator<<(std::ostream &os, const Point2 &point);
    };

    class Point3
    {
    private:
        double data[3];

    public:
        // Constructors
        Point3();
        Point3(double x, double y, double z);
        Point3(const Point3 &other);

        Point3 &operator=(const Point3 &other);

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

        // Point and vector operations
        Point3 operator+(const vector3d &vec) const;
        Point3 operator-(const vector3d &vec) const;
        vector3d operator-(const Point3 &other) const;

        Point3 &operator+=(const vector3d &vec);
        Point3 &operator-=(const vector3d &vec);

        double squaredDistance(const Point3 &other) const;
        double distance(const Point3 &other) const;

        // Utility functions
        bool operator==(const Point3 &other) const;
        bool operator!=(const Point3 &other) const;

        // Stream output
        friend std::ostream &operator<<(std::ostream &os, const Point3 &point);
    };
}
