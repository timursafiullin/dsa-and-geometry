#include "geox/geox.h"

#include <iostream>

namespace gx2 = geox::dim2;
namespace gx3 = geox::dim3;

int main()
{
    const gx2::Triangle triangle2{gx2::Point{}, gx2::Point{4.0, 0.0}, gx2::Point{0.0, 3.0}};
    const gx2::Point point2{1.0, 1.0};
    const auto barycentric2 = barycentricCoordinates(point2, triangle2);

    std::cout << "2D area: " << triangle2.area()
              << ", contains " << point2 << ": " << std::boolalpha
              << isPointInTriangle(point2, triangle2) << '\n';
    if (barycentric2)
        std::cout << "barycentric coordinates: (" << barycentric2->l0 << ", "
                  << barycentric2->l1 << ", " << barycentric2->l2 << ")\n";

    const gx3::Triangle triangle3{gx3::Point{}, gx3::Point{1.0, 0.0, 0.0}, gx3::Point{0.0, 1.0, 0.0}};
    const gx3::Point point3{0.8, 0.8, 2.0};
    std::cout << "3D area: " << triangle3.area()
              << ", normal: " << triangle3.normal() << '\n';
    std::cout << "closest point: " << closestPointOnTriangle(point3, triangle3) << '\n';
    return 0;
}
