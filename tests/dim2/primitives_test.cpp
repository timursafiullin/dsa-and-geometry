#include "test_framework.h"

#include "geox/geox.h"

#include <stdexcept>

namespace
{
    bool samePoint(const geox::dim2::Point& first, const geox::dim2::Point& second)
    {
        return first == second;
    }

    bool samePoint(const geox::dim3::Point& first, const geox::dim3::Point& second)
    {
        return first == second;
    }
} // namespace

GEOX_TEST(vectors_support_arithmetic_normalization_and_bounds_checks)
{
    using namespace geox;

    const dim2::Vector vector{3.0, 4.0};
    GEOX_REQUIRE(vector + dim2::Vector{1.0, -2.0} == dim2::Vector{4.0, 2.0});
    GEOX_REQUIRE(vector - dim2::Vector{1.0, 2.0} == dim2::Vector{2.0, 2.0});
    GEOX_REQUIRE(2.0 * vector == dim2::Vector{6.0, 8.0});
    GEOX_REQUIRE_CLOSE(vector.dot(dim2::Vector{1.0, 2.0}), 11.0);
    GEOX_REQUIRE_CLOSE(vector.cross(dim2::Vector{1.0, 2.0}), 2.0);
    GEOX_REQUIRE_CLOSE(vector.norm(), 5.0);
    GEOX_REQUIRE(vector.normalized() == dim2::Vector{0.6, 0.8});
    GEOX_REQUIRE(dim2::Vector{}.isZero());
    GEOX_REQUIRE_THROWS_AS((dim2::Vector{}.normalized()), std::invalid_argument);
    GEOX_REQUIRE_THROWS_AS(vector[2], std::out_of_range);
    GEOX_REQUIRE_THROWS_AS(vector / 0.0, std::invalid_argument);

    const dim3::Vector xAxis{1.0, 0.0, 0.0};
    const dim3::Vector yAxis{0.0, 1.0, 0.0};
    GEOX_REQUIRE(xAxis.cross(yAxis) == dim3::Vector{0.0, 0.0, 1.0});
    GEOX_REQUIRE_CLOSE(xAxis.dot(yAxis), 0.0);
    GEOX_REQUIRE_THROWS_AS(yAxis[-1], std::out_of_range);
}

GEOX_TEST(points_support_translation_distance_and_bounds_checks)
{
    using namespace geox;

    dim2::Point point2{1.0, 2.0};
    point2 += dim2::Vector{3.0, -1.0};
    GEOX_REQUIRE(samePoint(point2, dim2::Point{4.0, 1.0}));
    GEOX_REQUIRE(samePoint(point2 - dim2::Vector{2.0, 3.0}, dim2::Point{2.0, -2.0}));
    GEOX_REQUIRE(point2 - dim2::Point{1.0, 1.0} == dim2::Vector{3.0, 0.0});
    GEOX_REQUIRE_CLOSE(distance(dim2::Point{0.0, 0.0}, dim2::Point{3.0, 4.0}), 5.0);
    GEOX_REQUIRE_THROWS_AS(point2[2], std::out_of_range);

    dim3::Point point3{1.0, 2.0, 3.0};
    point3 -= dim3::Vector{1.0, -2.0, 3.0};
    GEOX_REQUIRE(samePoint(point3, dim3::Point{0.0, 4.0, 0.0}));
    GEOX_REQUIRE_CLOSE(distance(dim3::Point{}, dim3::Point{2.0, 3.0, 6.0}), 7.0);
    GEOX_REQUIRE_THROWS_AS(point3[3], std::out_of_range);
}

GEOX_TEST(segments_validate_interpolation_and_calculate_geometry)
{
    using namespace geox;

    const dim2::Segment segment2{dim2::Point{0.0, 0.0}, dim2::Point{4.0, 3.0}};
    GEOX_REQUIRE(segment2.direction() == dim2::Vector{4.0, 3.0});
    GEOX_REQUIRE_CLOSE(segment2.length(), 5.0);
    GEOX_REQUIRE(samePoint(segment2.pointAt(0.25), dim2::Point{1.0, 0.75}));
    GEOX_REQUIRE_THROWS_AS(segment2.pointAt(-0.01), std::invalid_argument);
    GEOX_REQUIRE_THROWS_AS(segment2.pointAt(1.01), std::invalid_argument);

    const dim3::Segment segment3{dim3::Point{0.0, 0.0, 0.0}, dim3::Point{0.0, 0.0, 2.0}};
    GEOX_REQUIRE_CLOSE(segment3.length(), 2.0);
    GEOX_REQUIRE(samePoint(segment3.pointAt(0.5), dim3::Point{0.0, 0.0, 1.0}));
}

GEOX_TEST(triangles_calculate_area_normals_and_degeneracy)
{
    using namespace geox;

    const dim2::Triangle triangle2{dim2::Point{0.0, 0.0}, dim2::Point{4.0, 0.0}, dim2::Point{0.0, 3.0}};
    GEOX_REQUIRE_CLOSE(triangle2.signedDoubleArea(), 12.0);
    GEOX_REQUIRE_CLOSE(triangle2.area(), 6.0);
    GEOX_REQUIRE_CLOSE(triangleArea(triangle2.a(), triangle2.b(), triangle2.c()), 6.0);
    GEOX_REQUIRE(!triangle2.isDegenerate());
    GEOX_REQUIRE(dim2::Triangle{dim2::Point{}, dim2::Point{1.0, 1.0}, dim2::Point{2.0, 2.0}}.isDegenerate());

    const dim3::Triangle triangle3{dim3::Point{}, dim3::Point{1.0, 0.0, 0.0}, dim3::Point{0.0, 1.0, 0.0}};
    GEOX_REQUIRE(triangle3.areaNormal() == dim3::Vector{0.0, 0.0, 1.0});
    GEOX_REQUIRE(triangle3.normal() == dim3::Vector{0.0, 0.0, 1.0});
    GEOX_REQUIRE_CLOSE(triangle3.area(), 0.5);
    GEOX_REQUIRE(!triangle3.isDegenerate());
}

GEOX_TEST(triangle_mesh_validates_indices_geometry_and_orientation)
{
    using namespace geox;

    dim3::TriangleMesh mesh;
    const dim3::VertexId first = mesh.addVertex(dim3::Point{0.0, 0.0, 0.0});
    const dim3::VertexId second = mesh.addVertex(dim3::Point{1.0, 0.0, 0.0});
    const dim3::VertexId third = mesh.addVertex(dim3::Point{0.0, 1.0, 0.0});
    GEOX_REQUIRE(mesh.addTriangle({first, second, third}) == 0);
    GEOX_REQUIRE(mesh.isValid());
    GEOX_REQUIRE(mesh.isGeometricallyValid());
    GEOX_REQUIRE_CLOSE(mesh.triangleArea(0), 0.5);
    GEOX_REQUIRE(mesh.triangleNormal(0) == dim3::Vector{0.0, 0.0, 1.0});
    mesh.reverseTriangleOrientation(0);
    GEOX_REQUIRE(mesh.triangle(0) == dim3::TriangleIndices{first, third, second});

    const dim3::TriangleMesh invalidIndices{{dim3::Point{}}, {dim3::TriangleIndices{0, 1, 2}}};
    GEOX_REQUIRE(!invalidIndices.isValid());
    const dim3::TriangleMesh degenerate{
        {dim3::Point{}, dim3::Point{1.0, 0.0, 0.0}, dim3::Point{2.0, 0.0, 0.0}},
        {dim3::TriangleIndices{0, 1, 2}}
    };
    GEOX_REQUIRE(degenerate.isValid());
    GEOX_REQUIRE(!degenerate.isGeometricallyValid());

    GEOX_REQUIRE_CLOSE(orient2d(dim2::Point{}, dim2::Point{1.0, 0.0}, dim2::Point{0.0, 1.0}), 1.0);
}
