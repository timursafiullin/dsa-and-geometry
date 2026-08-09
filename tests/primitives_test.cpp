#include "test_framework.h"

#include "geox/predicates.h"
#include "geox/primitives/points.h"
#include "geox/primitives/segments.h"
#include "geox/primitives/triangle_mesh.h"
#include "geox/primitives/triangles.h"
#include "geox/primitives/vectors.h"

#include <stdexcept>

namespace
{
    bool samePoint(const geox::Point2& first, const geox::Point2& second)
    {
        return first == second;
    }

    bool samePoint(const geox::Point3& first, const geox::Point3& second)
    {
        return first == second;
    }
} // namespace

GEOX_TEST(vectors_support_arithmetic_normalization_and_bounds_checks)
{
    using namespace geox;

    const vector2d vector{3.0, 4.0};
    GEOX_REQUIRE(vector + vector2d{1.0, -2.0} == vector2d{4.0, 2.0});
    GEOX_REQUIRE(vector - vector2d{1.0, 2.0} == vector2d{2.0, 2.0});
    GEOX_REQUIRE(2.0 * vector == vector2d{6.0, 8.0});
    GEOX_REQUIRE_CLOSE(vector.dot(vector2d{1.0, 2.0}), 11.0);
    GEOX_REQUIRE_CLOSE(vector.cross(vector2d{1.0, 2.0}), 2.0);
    GEOX_REQUIRE_CLOSE(vector.norm(), 5.0);
    GEOX_REQUIRE(vector.normalized() == vector2d{0.6, 0.8});
    GEOX_REQUIRE(vector2d{}.isZero());
    GEOX_REQUIRE_THROWS_AS((vector2d{}.normalized()), std::invalid_argument);
    GEOX_REQUIRE_THROWS_AS(vector[2], std::out_of_range);
    GEOX_REQUIRE_THROWS_AS(vector / 0.0, std::invalid_argument);

    const vector3d xAxis{1.0, 0.0, 0.0};
    const vector3d yAxis{0.0, 1.0, 0.0};
    GEOX_REQUIRE(xAxis.cross(yAxis) == vector3d{0.0, 0.0, 1.0});
    GEOX_REQUIRE_CLOSE(xAxis.dot(yAxis), 0.0);
    GEOX_REQUIRE_THROWS_AS(yAxis[-1], std::out_of_range);
}

GEOX_TEST(points_support_translation_distance_and_bounds_checks)
{
    using namespace geox;

    Point2 point2{1.0, 2.0};
    point2 += vector2d{3.0, -1.0};
    GEOX_REQUIRE(samePoint(point2, Point2{4.0, 1.0}));
    GEOX_REQUIRE(samePoint(point2 - vector2d{2.0, 3.0}, Point2{2.0, -2.0}));
    GEOX_REQUIRE(point2 - Point2{1.0, 1.0} == vector2d{3.0, 0.0});
    GEOX_REQUIRE_CLOSE(distance(Point2{0.0, 0.0}, Point2{3.0, 4.0}), 5.0);
    GEOX_REQUIRE_THROWS_AS(point2[2], std::out_of_range);

    Point3 point3{1.0, 2.0, 3.0};
    point3 -= vector3d{1.0, -2.0, 3.0};
    GEOX_REQUIRE(samePoint(point3, Point3{0.0, 4.0, 0.0}));
    GEOX_REQUIRE_CLOSE(distance(Point3{}, Point3{2.0, 3.0, 6.0}), 7.0);
    GEOX_REQUIRE_THROWS_AS(point3[3], std::out_of_range);
}

GEOX_TEST(segments_validate_interpolation_and_calculate_geometry)
{
    using namespace geox;

    const Segment2 segment2{Point2{0.0, 0.0}, Point2{4.0, 3.0}};
    GEOX_REQUIRE(segment2.direction() == vector2d{4.0, 3.0});
    GEOX_REQUIRE_CLOSE(segment2.length(), 5.0);
    GEOX_REQUIRE(samePoint(segment2.pointAt(0.25), Point2{1.0, 0.75}));
    GEOX_REQUIRE_THROWS_AS(segment2.pointAt(-0.01), std::invalid_argument);
    GEOX_REQUIRE_THROWS_AS(segment2.pointAt(1.01), std::invalid_argument);

    const Segment3 segment3{Point3{0.0, 0.0, 0.0}, Point3{0.0, 0.0, 2.0}};
    GEOX_REQUIRE_CLOSE(segment3.length(), 2.0);
    GEOX_REQUIRE(samePoint(segment3.pointAt(0.5), Point3{0.0, 0.0, 1.0}));
}

GEOX_TEST(triangles_calculate_area_normals_and_degeneracy)
{
    using namespace geox;

    const Triangle2 triangle2{Point2{0.0, 0.0}, Point2{4.0, 0.0}, Point2{0.0, 3.0}};
    GEOX_REQUIRE_CLOSE(triangle2.signedDoubleArea(), 12.0);
    GEOX_REQUIRE_CLOSE(triangle2.area(), 6.0);
    GEOX_REQUIRE_CLOSE(triangleArea(triangle2.a(), triangle2.b(), triangle2.c()), 6.0);
    GEOX_REQUIRE(!triangle2.isDegenerate());
    GEOX_REQUIRE(Triangle2{Point2{}, Point2{1.0, 1.0}, Point2{2.0, 2.0}}.isDegenerate());

    const Triangle3 triangle3{Point3{}, Point3{1.0, 0.0, 0.0}, Point3{0.0, 1.0, 0.0}};
    GEOX_REQUIRE(triangle3.areaNormal() == vector3d{0.0, 0.0, 1.0});
    GEOX_REQUIRE(triangle3.normal() == vector3d{0.0, 0.0, 1.0});
    GEOX_REQUIRE_CLOSE(triangle3.area(), 0.5);
    GEOX_REQUIRE(!triangle3.isDegenerate());
}

GEOX_TEST(triangle_mesh_validates_indices_geometry_and_orientation)
{
    using namespace geox;

    TriangleMesh mesh;
    const VertexId first = mesh.addVertex(Point3{0.0, 0.0, 0.0});
    const VertexId second = mesh.addVertex(Point3{1.0, 0.0, 0.0});
    const VertexId third = mesh.addVertex(Point3{0.0, 1.0, 0.0});
    GEOX_REQUIRE(mesh.addTriangle({first, second, third}) == 0);
    GEOX_REQUIRE(mesh.isValid());
    GEOX_REQUIRE(mesh.isGeometricallyValid());
    GEOX_REQUIRE_CLOSE(mesh.triangleArea(0), 0.5);
    GEOX_REQUIRE(mesh.triangleNormal(0) == vector3d{0.0, 0.0, 1.0});
    mesh.reverseTriangleOrientation(0);
    GEOX_REQUIRE(mesh.triangle(0) == TriangleIndices{first, third, second});

    const TriangleMesh invalidIndices{{Point3{}}, {TriangleIndices{0, 1, 2}}};
    GEOX_REQUIRE(!invalidIndices.isValid());
    const TriangleMesh degenerate{
        {Point3{}, Point3{1.0, 0.0, 0.0}, Point3{2.0, 0.0, 0.0}},
        {TriangleIndices{0, 1, 2}}
    };
    GEOX_REQUIRE(degenerate.isValid());
    GEOX_REQUIRE(!degenerate.isGeometricallyValid());
    GEOX_REQUIRE_CLOSE(orient2d(Point2{}, Point2{1.0, 0.0}, Point2{0.0, 1.0}), 1.0);
}
