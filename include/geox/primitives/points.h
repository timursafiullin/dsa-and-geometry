#pragma once

#include "geox/primitives/vectors.h"

#include <iosfwd>

namespace geox
{

    class Point2
    {
    private:
        double data[2];
    public:
        Point2();
        Point2(double x, double y);
        Point2(const Point2 &other);

        Point2 &operator=(const Point2 &other);

        double &x();
        double &y();
        const double &x() const;
        const double &y() const;

        double &operator[](int index);
        const double &operator[](int index) const;

        Point2 operator+(const vector2d &vec) const;
        Point2 operator-(const vector2d &vec) const;
        vector2d operator-(const Point2 &other) const;

        Point2 &operator+=(const vector2d &vec);
        Point2 &operator-=(const vector2d &vec);

        double squaredDistance(const Point2 &other) const;
        double distance(const Point2 &other) const;

        bool operator==(const Point2 &other) const;
        bool operator!=(const Point2 &other) const;

        friend std::ostream &operator<<(std::ostream &os, const Point2 &point);
    };

    double distance(const Point2& a, const Point2& b);

    class Point3
    {
    private:
        double data[3];
    public:
        Point3();
        Point3(double x, double y, double z);
        Point3(const Point3 &other);

        Point3 &operator=(const Point3 &other);

        double &x();
        double &y();
        double &z();
        const double &x() const;
        const double &y() const;
        const double &z() const;

        double &operator[](int index);
        const double &operator[](int index) const;

        Point3 operator+(const vector3d &vec) const;
        Point3 operator-(const vector3d &vec) const;
        vector3d operator-(const Point3 &other) const;

        Point3 &operator+=(const vector3d &vec);
        Point3 &operator-=(const vector3d &vec);

        double squaredDistance(const Point3 &other) const;
        double distance(const Point3 &other) const;

        bool operator==(const Point3 &other) const;
        bool operator!=(const Point3 &other) const;

        friend std::ostream &operator<<(std::ostream &os, const Point3 &point);
    };

    double distance(const Point3& a, const Point3& b);

} // namespace geox
