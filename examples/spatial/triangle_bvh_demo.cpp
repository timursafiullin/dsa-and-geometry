#include "geox/primitives/triangle_mesh.h"
#include "geox/spatial/aabb.h"
#include "geox/spatial/bounds.h"
#include "geox/spatial/triangle_bvh.h"

#include <iostream>

using namespace geox;

int main()
{
    const TriangleMesh cube{
        {
            Point3{0.0, 0.0, 0.0}, Point3{1.0, 0.0, 0.0},
            Point3{1.0, 1.0, 0.0}, Point3{0.0, 1.0, 0.0},
            Point3{0.0, 0.0, 1.0}, Point3{1.0, 0.0, 1.0},
            Point3{1.0, 1.0, 1.0}, Point3{0.0, 1.0, 1.0}
        },
        {
            TriangleIndices{0, 2, 1}, TriangleIndices{0, 3, 2},
            TriangleIndices{4, 5, 6}, TriangleIndices{4, 6, 7},
            TriangleIndices{0, 1, 5}, TriangleIndices{0, 5, 4},
            TriangleIndices{1, 2, 6}, TriangleIndices{1, 6, 5},
            TriangleIndices{2, 3, 7}, TriangleIndices{2, 7, 6},
            TriangleIndices{3, 0, 4}, TriangleIndices{3, 4, 7}
        }
    };

    const TriangleBVH bvh = buildTriangleBVH(cube);
    const Point3 query{1.7, 0.3, 0.6};
    const auto closest = bvh.closestPoint(query);
    const auto meshBounds = boundingBox(cube);
    const auto candidates = bvh.overlapCandidates(
        AABB3{Point3{0.95, 0.2, 0.2}, Point3{1.05, 0.8, 0.8}}
    );

    if (!closest.has_value() || !meshBounds.has_value())
        return 1;

    std::cout << "nodes: " << bvh.nodeCount()
              << ", triangles: " << bvh.triangleCount() << '\n';
    std::cout << "mesh bounds: " << meshBounds->min()
              << " to " << meshBounds->max() << '\n';
    std::cout << "closest point: " << closest->closestPoint
              << ", squared distance: " << closest->squaredDistance
              << ", triangle: " << closest->triangleId << '\n';
    std::cout << "right-side query candidates: ";
    for (TriangleId triangleId : candidates)
        std::cout << triangleId << ' ';
    std::cout << '\n';
    return 0;
}
