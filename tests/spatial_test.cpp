#include "test_framework.h"

#include "geox/algorithms/triangle_mesh.h"
#include "geox/primitives/segments.h"
#include "geox/primitives/triangle_mesh.h"
#include "geox/primitives/triangles.h"
#include "geox/spatial/aabb.h"
#include "geox/spatial/bounds.h"
#include "geox/spatial/triangle_bvh.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

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

    geox::TriangleMesh makeTwoLayerMesh()
    {
        return {
            {
                geox::Point3{0.0, 0.0, 0.0}, geox::Point3{1.0, 0.0, 0.0},
                geox::Point3{1.0, 1.0, 0.0}, geox::Point3{0.0, 1.0, 0.0},
                geox::Point3{0.0, 0.0, 2.0}, geox::Point3{1.0, 0.0, 2.0},
                geox::Point3{1.0, 1.0, 2.0}, geox::Point3{0.0, 1.0, 2.0}
            },
            {
                geox::TriangleIndices{0, 1, 2}, geox::TriangleIndices{0, 2, 3},
                geox::TriangleIndices{4, 6, 5}, geox::TriangleIndices{4, 7, 6}
            }
        };
    }

    std::vector<geox::TriangleId> sorted(std::vector<geox::TriangleId> ids)
    {
        std::sort(ids.begin(), ids.end());
        return ids;
    }
} // namespace

GEOX_TEST(aabb_supports_closed_bounds_distance_merging_and_axis_selection)
{
    using namespace geox;

    const AABB2 box2{Point2{-2.0, 1.0}, Point2{3.0, 5.0}};
    GEOX_REQUIRE(samePoint(box2.min(), Point2{-2.0, 1.0}));
    GEOX_REQUIRE(samePoint(box2.max(), Point2{3.0, 5.0}));
    GEOX_REQUIRE(samePoint(box2.center(), Point2{0.5, 3.0}));
    GEOX_REQUIRE(box2.extent() == vector2d{5.0, 4.0});
    GEOX_REQUIRE(box2.contains(Point2{-2.0, 1.0}));
    GEOX_REQUIRE(box2.contains(Point2{3.0, 5.0}));
    GEOX_REQUIRE(!box2.contains(Point2{3.01, 5.0}));
    GEOX_REQUIRE_CLOSE(box2.squaredDistance(Point2{0.0, 3.0}), 0.0);
    GEOX_REQUIRE_CLOSE(box2.squaredDistance(Point2{-4.0, 3.0}), 4.0);
    GEOX_REQUIRE_CLOSE(box2.squaredDistance(Point2{0.0, 8.0}), 9.0);
    GEOX_REQUIRE_CLOSE(box2.squaredDistance(Point2{-4.0, 8.0}), 13.0);
    GEOX_REQUIRE(box2.longestAxis() == 0);

    const AABB2 adjacent2{Point2{3.0, 4.0}, Point2{6.0, 8.0}};
    GEOX_REQUIRE(box2.overlaps(adjacent2));
    GEOX_REQUIRE(box2.overlaps(AABB2{Point2{3.0, 5.0}, Point2{4.0, 6.0}}));
    GEOX_REQUIRE(!box2.overlaps(AABB2{Point2{3.01, 1.0}, Point2{4.0, 2.0}}));
    const AABB2 merged2 = merge(box2, adjacent2);
    GEOX_REQUIRE(samePoint(merged2.min(), Point2{-2.0, 1.0}));
    GEOX_REQUIRE(samePoint(merged2.max(), Point2{6.0, 8.0}));

    const AABB3 box3{Point3{-1.0, -2.0, -3.0}, Point3{1.0, 2.0, 5.0}};
    GEOX_REQUIRE(samePoint(box3.center(), Point3{0.0, 0.0, 1.0}));
    GEOX_REQUIRE(box3.extent() == vector3d{2.0, 4.0, 8.0});
    GEOX_REQUIRE(box3.longestAxis() == 2);
    GEOX_REQUIRE(box3.contains(Point3{1.0, 2.0, 5.0}));
    GEOX_REQUIRE_CLOSE(box3.squaredDistance(Point3{3.0, 5.0, 9.0}), 29.0);
    GEOX_REQUIRE(box3.overlaps(AABB3{Point3{1.0, 2.0, 5.0}, Point3{2.0, 3.0, 6.0}}));
    GEOX_REQUIRE(box3.overlaps(AABB3{Point3{-0.5, -1.0, -2.0}, Point3{0.5, 1.0, 4.0}}));
    GEOX_REQUIRE(!box3.overlaps(AABB3{Point3{1.01, 2.0, 5.0}, Point3{2.0, 3.0, 6.0}}));

    GEOX_REQUIRE_THROWS_AS((AABB2{Point2{1.0, 0.0}, Point2{0.0, 1.0}}), std::invalid_argument);
    GEOX_REQUIRE_THROWS_AS((AABB3{Point3{0.0, 2.0, 0.0}, Point3{1.0, 1.0, 1.0}}), std::invalid_argument);
}

GEOX_TEST(bounds_cover_segments_triangles_and_meshes)
{
    using namespace geox;

    const AABB2 segment2Bounds = boundingBox(
        Segment2{Point2{3.0, -2.0}, Point2{-1.0, 5.0}}
    );
    GEOX_REQUIRE(samePoint(segment2Bounds.min(), Point2{-1.0, -2.0}));
    GEOX_REQUIRE(samePoint(segment2Bounds.max(), Point2{3.0, 5.0}));

    const AABB3 triangle3Bounds = boundingBox(Triangle3{
        Point3{2.0, -1.0, 3.0}, Point3{-4.0, 5.0, 1.0}, Point3{0.0, 2.0, 8.0}
    });
    GEOX_REQUIRE(samePoint(triangle3Bounds.min(), Point3{-4.0, -1.0, 1.0}));
    GEOX_REQUIRE(samePoint(triangle3Bounds.max(), Point3{2.0, 5.0, 8.0}));

    const TriangleMesh mesh = makeTwoLayerMesh();
    const AABB3 firstTriangleBounds = boundingBox(mesh, 0);
    GEOX_REQUIRE(samePoint(firstTriangleBounds.min(), Point3{0.0, 0.0, 0.0}));
    GEOX_REQUIRE(samePoint(firstTriangleBounds.max(), Point3{1.0, 1.0, 0.0}));

    const std::optional<AABB3> meshBounds = boundingBox(mesh);
    GEOX_REQUIRE(meshBounds.has_value());
    GEOX_REQUIRE(samePoint(meshBounds->min(), Point3{0.0, 0.0, 0.0}));
    GEOX_REQUIRE(samePoint(meshBounds->max(), Point3{1.0, 1.0, 2.0}));
    GEOX_REQUIRE(!boundingBox(TriangleMesh{}).has_value());
    GEOX_REQUIRE_THROWS_AS(boundingBox(mesh, 4), std::out_of_range);
}

GEOX_TEST(linear_mesh_closest_point_handles_empty_meshes_and_deterministic_ties)
{
    using namespace geox;

    GEOX_REQUIRE(!closestPointOnMesh(Point3{}, TriangleMesh{}).has_value());

    const TriangleMesh mesh = makeTwoLayerMesh();
    const auto aboveLowerLayer = closestPointOnMesh(Point3{0.25, 0.25, 0.4}, mesh);
    GEOX_REQUIRE(aboveLowerLayer.has_value());
    GEOX_REQUIRE(aboveLowerLayer->triangleId == 0);
    GEOX_REQUIRE(samePoint(aboveLowerLayer->closestPoint, Point3{0.25, 0.25, 0.0}));
    GEOX_REQUIRE_CLOSE(aboveLowerLayer->squaredDistance, 0.16);

    const TriangleMesh tiedMesh{
        {
            Point3{0.0, 0.0, -1.0}, Point3{1.0, 0.0, -1.0}, Point3{0.0, 1.0, -1.0},
            Point3{0.0, 0.0, 1.0}, Point3{1.0, 0.0, 1.0}, Point3{0.0, 1.0, 1.0}
        },
        {TriangleIndices{0, 1, 2}, TriangleIndices{3, 4, 5}}
    };
    const auto tied = closestPointOnMesh(Point3{0.25, 0.25, 0.0}, tiedMesh);
    GEOX_REQUIRE(tied.has_value());
    GEOX_REQUIRE(tied->triangleId == 0);
    GEOX_REQUIRE_CLOSE(tied->squaredDistance, 1.0);

    const TriangleMesh invalidMesh{{Point3{}}, {TriangleIndices{0, 1, 2}}};
    GEOX_REQUIRE_THROWS_AS(closestPointOnMesh(Point3{}, invalidMesh), std::invalid_argument);
}

GEOX_TEST(triangle_bvh_matches_linear_scan_and_returns_overlap_candidates)
{
    using namespace geox;

    const TriangleMesh mesh = makeTwoLayerMesh();
    const TriangleBVH bvh = buildTriangleBVH(mesh, TriangleBVHBuildOptions{1});
    GEOX_REQUIRE(!bvh.empty());
    GEOX_REQUIRE(bvh.triangleCount() == mesh.triangleCount());
    GEOX_REQUIRE(bvh.nodeCount() == 7);
    GEOX_REQUIRE(bvh.maxDepth() == 2);

    const std::vector<Point3> queries = {
        Point3{0.25, 0.25, 0.4}, Point3{0.75, 0.75, 1.8}, Point3{-1.0, 0.3, 1.0},
        Point3{2.0, -1.0, -2.0}, Point3{0.0, 0.0, 0.0}, Point3{0.5, 0.5, 1.0}
    };

    for (const Point3& query : queries)
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
                const Point3 query{0.25 * x, 0.25 * y, 0.5 * z};
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
        sorted(bvh.overlapCandidates(AABB3{Point3{-0.1, -0.1, -0.1}, Point3{1.1, 1.1, 0.1}})) ==
        std::vector<TriangleId>{0, 1}
    );

    const TriangleBVH wideLeafBvh = buildTriangleBVH(mesh);
    GEOX_REQUIRE(
        sorted(wideLeafBvh.overlapCandidates(
            AABB3{Point3{-0.1, -0.1, -0.1}, Point3{1.1, 1.1, 0.1}}
        )) == std::vector<TriangleId>{0, 1}
    );
    GEOX_REQUIRE(
        sorted(bvh.overlapCandidates(AABB3{Point3{0.0, 0.0, 0.0}, Point3{0.0, 0.0, 0.0}})) ==
        std::vector<TriangleId>{0, 1}
    );
    GEOX_REQUIRE(bvh.overlapCandidates(
        AABB3{Point3{2.0, 2.0, 2.0}, Point3{3.0, 3.0, 3.0}}
    ).empty());
}

GEOX_TEST(triangle_bvh_handles_empty_invalid_and_degenerate_build_options)
{
    using namespace geox;

    const TriangleBVH empty = buildTriangleBVH(TriangleMesh{});
    GEOX_REQUIRE(empty.empty());
    GEOX_REQUIRE(empty.nodeCount() == 0);
    GEOX_REQUIRE(empty.triangleCount() == 0);
    GEOX_REQUIRE(empty.maxDepth() == 0);
    GEOX_REQUIRE(!empty.closestPoint(Point3{}).has_value());
    GEOX_REQUIRE(empty.overlapCandidates(AABB3{Point3{}, Point3{}}).empty());

    const TriangleMesh mesh = makeTwoLayerMesh();
    GEOX_REQUIRE_THROWS_AS(
        buildTriangleBVH(mesh, TriangleBVHBuildOptions{0}),
        std::invalid_argument
    );

    const TriangleMesh invalidMesh{{Point3{}}, {TriangleIndices{0, 1, 2}}};
    GEOX_REQUIRE_THROWS_AS(buildTriangleBVH(invalidMesh), std::invalid_argument);
}
