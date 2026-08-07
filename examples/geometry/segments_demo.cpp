#include "geometry/primitives/segments.h"

#include <iostream>
#include <stdexcept>

using namespace dsa::geometry;

int main()
{
    std::cout << "=== Geometry segments demo ===\n\n";

    Segment2 path{Point2{1.0, 1.0}, Point2{7.0, 5.0}};

    std::cout << "2D segment\n";
    std::cout << "  start = " << path.start() << '\n';
    std::cout << "  end = " << path.end() << '\n';
    std::cout << "  direction = " << path.direction() << '\n';
    std::cout << "  length = " << path.length() << '\n';
    std::cout << "  halfway point = " << path.pointAt(0.5) << '\n';
    std::cout << "  point at t = 0.25 = " << path.pointAt(0.25) << "\n\n";

    Segment3 vertical{Point3{0.0, 0.0, 0.0}, Point3{0.0, 0.0, 10.0}};
    std::cout << "3D segment\n";
    std::cout << "  direction = " << vertical.direction() << '\n';
    std::cout << "  point at t = 0.7 = " << vertical.pointAt(0.7) << "\n\n";

    std::cout << "Invalid interpolation\n";
    try
    {
        path.pointAt(1.5);
    }
    catch (const std::invalid_argument& exception)
    {
        std::cout << "  Caught exception: " << exception.what() << '\n';
    }

    return 0;
}
