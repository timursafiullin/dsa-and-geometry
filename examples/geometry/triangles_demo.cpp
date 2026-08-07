#include "geometry/primitives/triangles.h"

#include <iostream>

using namespace dsa::geometry;

int main()
{
    std::cout << "=== Geometry triangles demo ===\n\n";

    Triangle2 triangle2{Point2{0.0, 0.0}, Point2{4.0, 0.0}, Point2{0.0, 3.0}};

    std::cout << "2D triangle\n";
    std::cout << "  edge AB = " << triangle2.edgeAB() << '\n';
    std::cout << "  edge AC = " << triangle2.edgeAC() << '\n';
    std::cout << "  signed double area = " << triangle2.signedDoubleArea() << '\n';
    std::cout << "  area = " << triangle2.area() << '\n';
    std::cout << "  degenerate = " << std::boolalpha << triangle2.isDegenerate() << "\n\n";

    Triangle3 triangle3{
        Point3{0.0, 0.0, 0.0},
        Point3{1.0, 0.0, 0.0},
        Point3{0.0, 1.0, 0.0},
    };

    std::cout << "3D triangle\n";
    std::cout << "  area normal = " << triangle3.areaNormal() << '\n';
    std::cout << "  unit normal = " << triangle3.normal() << '\n';
    std::cout << "  area = " << triangle3.area() << '\n';

    Triangle2 flat{Point2{0.0, 0.0}, Point2{1.0, 1.0}, Point2{2.0, 2.0}};
    std::cout << "\nCollinear triangle is degenerate = " << flat.isDegenerate() << '\n';

    return 0;
}
