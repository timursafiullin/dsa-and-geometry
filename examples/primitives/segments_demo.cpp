#include "geox/primitives/segments.h"

#include <iostream>

using namespace geox;

int main()
{
    const Segment2 path{Point2{1.0, 1.0}, Point2{7.0, 5.0}};
    const Segment3 vertical{Point3{}, Point3{0.0, 0.0, 10.0}};

    std::cout << "2D path: " << path.start() << " -> " << path.end() << '\n';
    std::cout << "length: " << path.length() << ", midpoint: " << path.pointAt(0.5) << '\n';
    std::cout << "3D point at t=0.7: " << vertical.pointAt(0.7) << '\n';
    return 0;
}
