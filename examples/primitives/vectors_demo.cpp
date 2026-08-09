#include "geox/primitives/vectors.h"

#include <iostream>

using namespace geox;

int main()
{
    std::cout << "=== GEOX vectors demo ===\n\n";

    vector2d east{3.0, 0.0};
    vector2d north{0.0, 4.0};

    std::cout << "2D vectors\n";
    std::cout << "  east  = " << east << '\n';
    std::cout << "  north = " << north << '\n';
    std::cout << "  east + north = " << east + north << '\n';
    std::cout << "  dot product  = " << east.dot(north) << '\n';
    std::cout << "  cross product = " << east.cross(north) << '\n';
    std::cout << "  |east + north| = " << (east + north).norm() << '\n';
    std::cout << "  normalized (3, 4) = " << vector2d{3.0, 4.0}.normalized() << "\n\n";

    vector3d xAxis{1.0, 0.0, 0.0};
    vector3d yAxis{0.0, 1.0, 0.0};

    std::cout << "3D vectors\n";
    std::cout << "  x-axis cross y-axis = " << xAxis.cross(yAxis) << '\n';
    std::cout << "  2 * (x-axis + y-axis) = " << 2.0 * (xAxis + yAxis) << '\n';

    return 0;
}
