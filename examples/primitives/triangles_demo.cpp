#include "geox/algorithms/triangle2.h"
#include "geox/algorithms/triangle3.h"

#include <iostream>

using namespace geox;

int main()
{
    const Triangle2 triangle2{Point2{}, Point2{4.0, 0.0}, Point2{0.0, 3.0}};
    const Point2 point2{1.0, 1.0};
    const auto barycentric2 = barycentricCoordinates(point2, triangle2);

    std::cout << "2D area: " << triangle2.area()
              << ", contains " << point2 << ": " << std::boolalpha
              << isPointInTriangle(point2, triangle2) << '\n';
    if (barycentric2)
        std::cout << "barycentric coordinates: (" << barycentric2->l0 << ", "
                  << barycentric2->l1 << ", " << barycentric2->l2 << ")\n";

    const Triangle3 triangle3{Point3{}, Point3{1.0, 0.0, 0.0}, Point3{0.0, 1.0, 0.0}};
    const Point3 point3{0.8, 0.8, 2.0};
    std::cout << "3D area: " << triangle3.area()
              << ", normal: " << triangle3.normal() << '\n';
    std::cout << "closest point: " << closestPointOnTriangle(point3, triangle3) << '\n';
    return 0;
}
