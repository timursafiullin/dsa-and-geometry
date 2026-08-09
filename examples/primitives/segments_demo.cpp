#include "geox/algorithms/segment2.h"
#include "geox/primitives/segments.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <variant>

using namespace geox;

namespace
{

    constexpr double kTestEpsilon = 1e-9;

    bool areClose(double first, double second)
    {
        return std::abs(first - second) <= kTestEpsilon;
    }

    bool areEqual(const Point2& first, const Point2& second)
    {
        return areClose(first.x(), second.x()) && areClose(first.y(), second.y());
    }

    void expect(bool condition, const char* message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }

    void expectPointIntersection(
        const SegmentIntersection2& result,
        const Point2& expected
    )
    {
        const Point2* point = std::get_if<Point2>(&result);
        expect(point != nullptr && areEqual(*point, expected),
               "Unexpected point intersection result.");
    }

    void expectSegmentIntersection(
        const SegmentIntersection2& result,
        const Segment2& expected
    )
    {
        const Segment2* segment = std::get_if<Segment2>(&result);
        expect(segment != nullptr &&
                   areEqual(segment->start(), expected.start()) &&
                   areEqual(segment->end(), expected.end()),
               "Unexpected overlapping segment result.");
    }

    void runSegmentAlgorithmTests()
    {
        const Segment2 horizontal{Point2{0.0, 0.0}, Point2{4.0, 0.0}};

        expect(isPointOnSegment(Point2{2.0, 0.0}, horizontal),
               "A point inside a segment must be recognized.");
        expect(!isPointOnSegment(Point2{2.0, -0.1}, horizontal),
               "A point off the supporting line must be rejected.");
        expect(areEqual(closestPointOnSegment(Point2{2.0, 3.0}, horizontal), Point2{2.0, 0.0}),
               "Closest point in the interior is incorrect.");

        const Segment2 pointSegment{Point2{1.0, 1.0}, Point2{1.0, 1.0}};
        expect(areEqual(closestPointOnSegment(Point2{3.0, 4.0}, pointSegment), Point2{1.0, 1.0}),
               "A degenerate segment must project to its only point.");

        expectPointIntersection(
            intersect(Segment2{Point2{0.0, 0.0}, Point2{4.0, 4.0}},
                      Segment2{Point2{0.0, 4.0}, Point2{4.0, 0.0}}),
            Point2{2.0, 2.0}
        );

        expectPointIntersection(
            intersect(Segment2{Point2{0.0, 0.0}, Point2{2.0, 0.0}},
                      Segment2{Point2{2.0, 0.0}, Point2{2.0, 3.0}}),
            Point2{2.0, 0.0}
        );

        expect(std::holds_alternative<std::monostate>(
                   intersect(Segment2{Point2{0.0, 0.0}, Point2{1.0, 0.0}},
                             Segment2{Point2{0.0, 1.0}, Point2{1.0, 1.0}})),
               "Parallel disjoint segments must not intersect.");

        expectSegmentIntersection(
            intersect(Segment2{Point2{0.0, 0.0}, Point2{5.0, 0.0}},
                      Segment2{Point2{4.0, 0.0}, Point2{2.0, 0.0}}),
            Segment2{Point2{2.0, 0.0}, Point2{4.0, 0.0}}
        );

        expect(std::holds_alternative<std::monostate>(
                   intersect(Segment2{Point2{0.0, 0.0}, Point2{1.0, 0.0}},
                             Segment2{Point2{2.0, 0.0}, Point2{3.0, 0.0}})),
               "Collinear disjoint segments must not intersect.");

        expectPointIntersection(
            intersect(Segment2{Point2{1.0, 0.0}, Point2{1.0, 0.0}}, horizontal),
            Point2{1.0, 0.0}
        );
    }

} // namespace

int main()
{
    std::cout << "=== GEOX segments demo ===\n\n";

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

    runSegmentAlgorithmTests();
    std::cout << "\nSegment algorithm tests passed.\n";

    return 0;
}
