#include "geox/geox.h"

#include <iostream>

namespace gx2 = geox::dim2;
namespace gx3 = geox::dim3;

int main()
{
    std::cout << "=== GEOX points demo ===\n\n";

    gx2::Point start{1.0, 2.0};
    gx2::Point finish{4.0, 6.0};
    gx2::Vector displacement = finish - start;

    std::cout << "2D points\n";
    std::cout << "  start = " << start << '\n';
    std::cout << "  finish = " << finish << '\n';
    std::cout << "  displacement = " << displacement << '\n';
    std::cout << "  distance = " << start.distance(finish) << '\n';
    std::cout << "  start translated by (2, -1) = "
              << start + gx2::Vector{2.0, -1.0} << "\n\n";

    gx3::Point origin{0.0, 0.0, 0.0};
    gx3::Point point{2.0, -1.0, 2.0};

    std::cout << "3D points\n";
    std::cout << "  point = " << point << '\n';
    std::cout << "  distance from origin = " << distance(origin, point) << '\n';
    point += gx3::Vector{1.0, 1.0, -2.0};
    std::cout << "  after translation = " << point << '\n';

    return 0;
}
