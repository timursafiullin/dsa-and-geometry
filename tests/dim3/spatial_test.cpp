#include "test_framework.h"

#include "geox/geox.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

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

    geox::dim3::TriangleMesh makeTwoLayerMesh()
    {
        return {
            {
                geox::dim3::Point{0.0, 0.0, 0.0}, geox::dim3::Point{1.0, 0.0, 0.0},
                geox::dim3::Point{1.0, 1.0, 0.0}, geox::dim3::Point{0.0, 1.0, 0.0},
                geox::dim3::Point{0.0, 0.0, 2.0}, geox::dim3::Point{1.0, 0.0, 2.0},
                geox::dim3::Point{1.0, 1.0, 2.0}, geox::dim3::Point{0.0, 1.0, 2.0}
            },
            {
                geox::dim3::TriangleIndices{0, 1, 2}, geox::dim3::TriangleIndices{0, 2, 3},
                geox::dim3::TriangleIndices{4, 6, 5}, geox::dim3::TriangleIndices{4, 7, 6}
            }
        };
    }

    std::vector<geox::dim3::TriangleId> sorted(std::vector<geox::dim3::TriangleId> ids)
    {
        std::sort(ids.begin(), ids.end());
        return ids;
    }
} // namespace

GEOX_TEST(aabb_supports_closed_bounds_distance_merging_and_axis_selection)
{
    using namespace geox;

    const dim2::AABB box2{dim2::Point{-2.0, 1.0}, dim2::Point{3.0, 5.0}};
    GEOX_REQUIRE(samePoint(box2.min(), dim2::Point{-2.0, 1.0}));
    GEOX_REQUIRE(samePoint(box2.max(), dim2::Point{3.0, 5.0}));
    GEOX_REQUIRE(samePoint(box2.center(), dim2::Point{0.5, 3.0}));
    GEOX_REQUIRE(box2.extent() == dim2::Vector{5.0, 4.0});
    GEOX_REQUIRE(box2.contains(dim2::Point{-2.0, 1.0}));
    GEOX_REQUIRE(box2.contains(dim2::Point{3.0, 5.0}));
    GEOX_REQUIRE(!box2.contains(dim2::Point{3.01, 5.0}));
    GEOX_REQUIRE_CLOSE(box2.squaredDistance(dim2::Point{0.0, 3.0}), 0.0);
    GEOX_REQUIRE_CLOSE(box2.squaredDistance(dim2::Point{-4.0, 3.0}), 4.0);
    GEOX_REQUIRE_CLOSE(box2.squaredDistance(dim2::Point{0.0, 8.0}), 9.0);
    GEOX_REQUIRE_CLOSE(box2.squaredDistance(dim2::Point{-4.0, 8.0}), 13.0);
    GEOX_REQUIRE(box2.longestAxis() == 0);

    const dim2::AABB adjacent2{dim2::Point{3.0, 4.0}, dim2::Point{6.0, 8.0}};
    GEOX_REQUIRE(box2.overlaps(adjacent2));
    GEOX_REQUIRE(box2.overlaps(dim2::AABB{dim2::Point{3.0, 5.0}, dim2::Point{4.0, 6.0}}));
    GEOX_REQUIRE(!box2.overlaps(dim2::AABB{dim2::Point{3.01, 1.0}, dim2::Point{4.0, 2.0}}));
    const dim2::AABB merged2 = merge(box2, adjacent2);
    GEOX_REQUIRE(samePoint(merged2.min(), dim2::Point{-2.0, 1.0}));
    GEOX_REQUIRE(samePoint(merged2.max(), dim2::Point{6.0, 8.0}));

    const dim3::AABB box3{dim3::Point{-1.0, -2.0, -3.0}, dim3::Point{1.0, 2.0, 5.0}};
    GEOX_REQUIRE(samePoint(box3.center(), dim3::Point{0.0, 0.0, 1.0}));
    GEOX_REQUIRE(box3.extent() == dim3::Vector{2.0, 4.0, 8.0});
    GEOX_REQUIRE(box3.longestAxis() == 2);
    GEOX_REQUIRE(box3.contains(dim3::Point{1.0, 2.0, 5.0}));
    GEOX_REQUIRE_CLOSE(box3.squaredDistance(dim3::Point{3.0, 5.0, 9.0}), 29.0);
    GEOX_REQUIRE(box3.overlaps(dim3::AABB{dim3::Point{1.0, 2.0, 5.0}, dim3::Point{2.0, 3.0, 6.0}}));
    GEOX_REQUIRE(box3.overlaps(dim3::AABB{dim3::Point{-0.5, -1.0, -2.0}, dim3::Point{0.5, 1.0, 4.0}}));
    GEOX_REQUIRE(!box3.overlaps(dim3::AABB{dim3::Point{1.01, 2.0, 5.0}, dim3::Point{2.0, 3.0, 6.0}}));

    GEOX_REQUIRE_THROWS_AS((dim2::AABB{dim2::Point{1.0, 0.0}, dim2::Point{0.0, 1.0}}), std::invalid_argument);
    GEOX_REQUIRE_THROWS_AS((dim3::AABB{dim3::Point{0.0, 2.0, 0.0}, dim3::Point{1.0, 1.0, 1.0}}), std::invalid_argument);
}

GEOX_TEST(bounds_cover_segments_triangles_and_meshes)
{
    using namespace geox;

    const dim2::AABB segment2Bounds = boundingBox(
        dim2::Segment{dim2::Point{3.0, -2.0}, dim2::Point{-1.0, 5.0}}
    );
    GEOX_REQUIRE(samePoint(segment2Bounds.min(), dim2::Point{-1.0, -2.0}));
    GEOX_REQUIRE(samePoint(segment2Bounds.max(), dim2::Point{3.0, 5.0}));

    const dim3::AABB triangle3Bounds = boundingBox(dim3::Triangle{
        dim3::Point{2.0, -1.0, 3.0}, dim3::Point{-4.0, 5.0, 1.0}, dim3::Point{0.0, 2.0, 8.0}
    });
    GEOX_REQUIRE(samePoint(triangle3Bounds.min(), dim3::Point{-4.0, -1.0, 1.0}));
    GEOX_REQUIRE(samePoint(triangle3Bounds.max(), dim3::Point{2.0, 5.0, 8.0}));

    const dim3::TriangleMesh mesh = makeTwoLayerMesh();
    const dim3::AABB firstTriangleBounds = boundingBox(mesh, 0);
    GEOX_REQUIRE(samePoint(firstTriangleBounds.min(), dim3::Point{0.0, 0.0, 0.0}));
    GEOX_REQUIRE(samePoint(firstTriangleBounds.max(), dim3::Point{1.0, 1.0, 0.0}));

    const std::optional<dim3::AABB> meshBounds = boundingBox(mesh);
    GEOX_REQUIRE(meshBounds.has_value());
    GEOX_REQUIRE(samePoint(meshBounds->min(), dim3::Point{0.0, 0.0, 0.0}));
    GEOX_REQUIRE(samePoint(meshBounds->max(), dim3::Point{1.0, 1.0, 2.0}));
    GEOX_REQUIRE(!boundingBox(dim3::TriangleMesh{}).has_value());
    GEOX_REQUIRE_THROWS_AS(boundingBox(mesh, 4), std::out_of_range);
}

GEOX_TEST(linear_mesh_closest_point_handles_empty_meshes_and_deterministic_ties)
{
    using namespace geox;

    GEOX_REQUIRE(!closestPointOnMesh(dim3::Point{}, dim3::TriangleMesh{}).has_value());

    const dim3::TriangleMesh mesh = makeTwoLayerMesh();
    const auto aboveLowerLayer = closestPointOnMesh(dim3::Point{0.25, 0.25, 0.4}, mesh);
    GEOX_REQUIRE(aboveLowerLayer.has_value());
    GEOX_REQUIRE(aboveLowerLayer->triangleId == 0);
    GEOX_REQUIRE(samePoint(aboveLowerLayer->closestPoint, dim3::Point{0.25, 0.25, 0.0}));
    GEOX_REQUIRE_CLOSE(aboveLowerLayer->squaredDistance, 0.16);

    const dim3::TriangleMesh tiedMesh{
        {
            dim3::Point{0.0, 0.0, -1.0}, dim3::Point{1.0, 0.0, -1.0}, dim3::Point{0.0, 1.0, -1.0},
            dim3::Point{0.0, 0.0, 1.0}, dim3::Point{1.0, 0.0, 1.0}, dim3::Point{0.0, 1.0, 1.0}
        },
        {dim3::TriangleIndices{0, 1, 2}, dim3::TriangleIndices{3, 4, 5}}
    };
    const auto tied = closestPointOnMesh(dim3::Point{0.25, 0.25, 0.0}, tiedMesh);
    GEOX_REQUIRE(tied.has_value());
    GEOX_REQUIRE(tied->triangleId == 0);
    GEOX_REQUIRE_CLOSE(tied->squaredDistance, 1.0);

    const dim3::TriangleMesh invalidMesh{{dim3::Point{}}, {dim3::TriangleIndices{0, 1, 2}}};
    GEOX_REQUIRE_THROWS_AS(closestPointOnMesh(dim3::Point{}, invalidMesh), std::invalid_argument);
}

GEOX_TEST(triangle_bvh_matches_linear_scan_and_returns_overlap_candidates)
{
    using namespace geox;

    const dim3::TriangleMesh mesh = makeTwoLayerMesh();
    const dim3::TriangleBVH bvh = buildTriangleBVH(mesh, dim3::TriangleBVHBuildOptions{1});
    GEOX_REQUIRE(!bvh.empty());
    GEOX_REQUIRE(bvh.triangleCount() == mesh.triangleCount());
    GEOX_REQUIRE(bvh.nodeCount() == 7);
    GEOX_REQUIRE(bvh.maxDepth() == 2);

    const std::vector<dim3::Point> queries = {
        dim3::Point{0.25, 0.25, 0.4}, dim3::Point{0.75, 0.75, 1.8}, dim3::Point{-1.0, 0.3, 1.0},
        dim3::Point{2.0, -1.0, -2.0}, dim3::Point{0.0, 0.0, 0.0}, dim3::Point{0.5, 0.5, 1.0}
    };

    for (const dim3::Point& query : queries)
    {
        const auto expected = closestPointOnMesh(query, mesh);
        const auto actual = bvh.closestPoint(query);
        GEOX_REQUIRE(expected.has_value());
        GEOX_REQUIRE(actual.has_value());
        GEOX_REQUIRE(actual->triangleId == expected->triangleId);
        GEOX_REQUIRE(samePoint(actual->closestPoint, expected->closestPoint));
        GEOX_REQUIRE_CLOSE(actual->squaredDistance, expected->squaredDistance);
    }

    for (int x = -2; x <= 4; ++x)
    {
        for (int y = -2; y <= 4; ++y)
        {
            for (int z = -2; z <= 4; ++z)
            {
                const dim3::Point query{0.25 * x, 0.25 * y, 0.5 * z};
                const auto expected = closestPointOnMesh(query, mesh);
                const auto actual = bvh.closestPoint(query);
                GEOX_REQUIRE(actual.has_value());
                GEOX_REQUIRE(actual->triangleId == expected->triangleId);
                GEOX_REQUIRE(samePoint(actual->closestPoint, expected->closestPoint));
                GEOX_REQUIRE_CLOSE(actual->squaredDistance, expected->squaredDistance);
            }
        }
    }

    GEOX_REQUIRE(
        sorted(bvh.overlapCandidates(dim3::AABB{dim3::Point{-0.1, -0.1, -0.1}, dim3::Point{1.1, 1.1, 0.1}})) ==
        std::vector<dim3::TriangleId>{0, 1}
    );

    const dim3::TriangleBVH wideLeafBvh = buildTriangleBVH(mesh);
    GEOX_REQUIRE(
        sorted(wideLeafBvh.overlapCandidates(
            dim3::AABB{dim3::Point{-0.1, -0.1, -0.1}, dim3::Point{1.1, 1.1, 0.1}}
        )) == std::vector<dim3::TriangleId>{0, 1}
    );
    GEOX_REQUIRE(
        sorted(bvh.overlapCandidates(dim3::AABB{dim3::Point{0.0, 0.0, 0.0}, dim3::Point{0.0, 0.0, 0.0}})) ==
        std::vector<dim3::TriangleId>{0, 1}
    );
    GEOX_REQUIRE(bvh.overlapCandidates(
        dim3::AABB{dim3::Point{2.0, 2.0, 2.0}, dim3::Point{3.0, 3.0, 3.0}}
    ).empty());
}

GEOX_TEST(triangle_bvh_handles_empty_invalid_and_degenerate_build_options)
{
    using namespace geox;

    const dim3::TriangleBVH empty = buildTriangleBVH(dim3::TriangleMesh{});
    GEOX_REQUIRE(empty.empty());
    GEOX_REQUIRE(empty.nodeCount() == 0);
    GEOX_REQUIRE(empty.triangleCount() == 0);
    GEOX_REQUIRE(empty.maxDepth() == 0);
    GEOX_REQUIRE(!empty.closestPoint(dim3::Point{}).has_value());
    GEOX_REQUIRE(empty.overlapCandidates(dim3::AABB{dim3::Point{}, dim3::Point{}}).empty());

    const dim3::TriangleMesh mesh = makeTwoLayerMesh();
    GEOX_REQUIRE_THROWS_AS(
        buildTriangleBVH(mesh, dim3::TriangleBVHBuildOptions{0}),
        std::invalid_argument
    );

    const dim3::TriangleMesh invalidMesh{{dim3::Point{}}, {dim3::TriangleIndices{0, 1, 2}}};
    GEOX_REQUIRE_THROWS_AS(buildTriangleBVH(invalidMesh), std::invalid_argument);
}
