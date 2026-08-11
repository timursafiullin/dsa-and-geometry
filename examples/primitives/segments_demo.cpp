#include "geox/geox.h"

#include <iostream>

namespace gx2 = geox::dim2;
namespace gx3 = geox::dim3;

int main()
{
    const gx2::Segment path{gx2::Point{1.0, 1.0}, gx2::Point{7.0, 5.0}};
    const gx3::Segment vertical{gx3::Point{}, gx3::Point{0.0, 0.0, 10.0}};

    std::cout << "2D path: " << path.start() << " -> " << path.end() << '\n';
    std::cout << "length: " << path.length() << ", midpoint: " << path.pointAt(0.5) << '\n';
    std::cout << "3D point at t=0.7: " << vertical.pointAt(0.7) << '\n';
    return 0;
}
