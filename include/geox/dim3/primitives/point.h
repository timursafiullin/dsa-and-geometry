#pragma once

#include "geox/dim3/primitives/vector.h"

#include <iosfwd>

namespace geox::dim3
{

    class Point
    {
    private:
        double data[3];
    public:
        Point();
        Point(double x, double y, double z);
        Point(const Point &other);

        Point &operator=(const Point &other);

        double &x();
        double &y();
        double &z();
        const double &x() const;
        const double &y() const;
        const double &z() const;

        double &operator[](int index);
        const double &operator[](int index) const;

        Point operator+(const Vector &vec) const;
        Point operator-(const Vector &vec) const;
        Vector operator-(const Point &other) const;

        Point &operator+=(const Vector &vec);
        Point &operator-=(const Vector &vec);

        double squaredDistance(const Point &other) const;
        double distance(const Point &other) const;

        bool operator==(const Point &other) const;
        bool operator!=(const Point &other) const;

        friend std::ostream &operator<<(std::ostream &os, const Point &point);
    };

    double distance(const Point& a, const Point& b);

} // namespace geox::dim3
