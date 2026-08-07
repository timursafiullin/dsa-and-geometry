#include "geometry/primitives/points.h"

#include <iostream>

using namespace dsa::geometry;

int main()
{
    std::cout << "=== Geometry points demo ===\n\n";

    Point2 start{1.0, 2.0};
    Point2 finish{4.0, 6.0};
    vector2d displacement = finish - start;

    std::cout << "2D points\n";
    std::cout << "  start = " << start << '\n';
    std::cout << "  finish = " << finish << '\n';
    std::cout << "  displacement = " << displacement << '\n';
    std::cout << "  distance = " << start.distance(finish) << '\n';
    std::cout << "  start translated by (2, -1) = "
              << start + vector2d{2.0, -1.0} << "\n\n";

    Point3 origin{0.0, 0.0, 0.0};
    Point3 point{2.0, -1.0, 2.0};

    std::cout << "3D points\n";
    std::cout << "  point = " << point << '\n';
    std::cout << "  distance from origin = " << distance(origin, point) << '\n';
    point += vector3d{1.0, 1.0, -2.0};
    std::cout << "  after translation = " << point << '\n';

    return 0;
}
