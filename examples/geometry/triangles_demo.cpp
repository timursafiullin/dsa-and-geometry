#include "geometry/algorithms/triangle2.h"
#include "geometry/algorithms/triangle3.h"
#include "geometry/primitives/triangles.h"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace dsa::geometry;

namespace
{

    bool isNearlyEqual(double lhs, double rhs)
    {
        return std::abs(lhs - rhs) <= kBarycentricEpsilon;
    }

} // namespace

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

    const Point2 insidePoint{1.0, 1.0};
    const std::optional<BarycentricCoordinates2> insideCoordinates =
        barycentricCoordinates(insidePoint, triangle2);

    assert(insideCoordinates.has_value());
    assert(isNearlyEqual(insideCoordinates->l0, 5.0 / 12.0));
    assert(isNearlyEqual(insideCoordinates->l1, 1.0 / 4.0));
    assert(isNearlyEqual(insideCoordinates->l2, 1.0 / 3.0));
    assert(isNearlyEqual(
        insideCoordinates->l0 + insideCoordinates->l1 + insideCoordinates->l2,
        1.0
    ));

    assert(isPointInTriangle(insidePoint, triangle2));
    assert(isPointInTriangle(triangle2.a(), triangle2));
    assert(isPointInTriangle(Point2{2.0, 0.0}, triangle2));
    assert(!isPointInTriangle(Point2{5.0, 0.0}, triangle2));

    Triangle3 triangle3{
        Point3{0.0, 0.0, 0.0},
        Point3{1.0, 0.0, 0.0},
        Point3{0.0, 1.0, 0.0},
    };

    std::cout << "3D triangle\n";
    std::cout << "  area normal = " << triangle3.areaNormal() << '\n';
    std::cout << "  unit normal = " << triangle3.normal() << '\n';
    std::cout << "  area = " << triangle3.area() << '\n';

    const Point3 insidePoint3{0.25, 0.25, 0.0};
    const std::optional<BarycentricCoordinates3> insideCoordinates3 =
        barycentricCoordinates(insidePoint3, triangle3);

    assert(insideCoordinates3.has_value());
    assert(isNearlyEqual(insideCoordinates3->l0, 0.5));
    assert(isNearlyEqual(insideCoordinates3->l1, 0.25));
    assert(isNearlyEqual(insideCoordinates3->l2, 0.25));
    assert(isPointInTriangle(insidePoint3, triangle3));
    assert(isPointInTriangle(triangle3.a(), triangle3));
    assert(!isPointInTriangle(Point3{1.0, 1.0, 0.0}, triangle3));
    assert(!barycentricCoordinates(Point3{0.25, 0.25, 0.1}, triangle3).has_value());
    assert(!isPointInTriangle(Point3{0.25, 0.25, 0.1}, triangle3));

    Triangle2 flat{Point2{0.0, 0.0}, Point2{1.0, 1.0}, Point2{2.0, 2.0}};
    std::cout << "\nCollinear triangle is degenerate = " << flat.isDegenerate() << '\n';

    assert(!barycentricCoordinates(Point2{1.0, 1.0}, flat).has_value());
    assert(!isPointInTriangle(Point2{1.0, 1.0}, flat));

    const Triangle3 flat3{
        Point3{0.0, 0.0, 0.0},
        Point3{1.0, 0.0, 0.0},
        Point3{2.0, 0.0, 0.0},
    };
    assert(!barycentricCoordinates(Point3{1.0, 0.0, 0.0}, flat3).has_value());
    assert(!isPointInTriangle(Point3{1.0, 0.0, 0.0}, flat3));

    std::cout << "Barycentric-coordinate checks passed.\n";

    return 0;
}
