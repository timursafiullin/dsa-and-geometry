#include "geox/geox.h"

#include <iostream>

namespace gx2 = geox::dim2;
namespace gx3 = geox::dim3;

int main()
{
    std::cout << "=== GEOX vectors demo ===\n\n";

    gx2::Vector east{3.0, 0.0};
    gx2::Vector north{0.0, 4.0};

    std::cout << "2D vectors\n";
    std::cout << "  east  = " << east << '\n';
    std::cout << "  north = " << north << '\n';
    std::cout << "  east + north = " << east + north << '\n';
    std::cout << "  dot product  = " << east.dot(north) << '\n';
    std::cout << "  cross product = " << east.cross(north) << '\n';
    std::cout << "  |east + north| = " << (east + north).norm() << '\n';
    std::cout << "  normalized (3, 4) = " << gx2::Vector{3.0, 4.0}.normalized() << "\n\n";

    gx3::Vector xAxis{1.0, 0.0, 0.0};
    gx3::Vector yAxis{0.0, 1.0, 0.0};

    std::cout << "3D vectors\n";
    std::cout << "  x-axis cross y-axis = " << xAxis.cross(yAxis) << '\n';
    std::cout << "  2 * (x-axis + y-axis) = " << 2.0 * (xAxis + yAxis) << '\n';

    return 0;
}
