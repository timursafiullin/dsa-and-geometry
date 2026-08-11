#include "geox/geox.h"

#include <iostream>

namespace gx2 = geox::dim2;
namespace gx3 = geox::dim3;

int main()
{
    const gx3::TriangleMesh cube{
        {
            gx3::Point{0.0, 0.0, 0.0}, gx3::Point{1.0, 0.0, 0.0},
            gx3::Point{1.0, 1.0, 0.0}, gx3::Point{0.0, 1.0, 0.0},
            gx3::Point{0.0, 0.0, 1.0}, gx3::Point{1.0, 0.0, 1.0},
            gx3::Point{1.0, 1.0, 1.0}, gx3::Point{0.0, 1.0, 1.0}
        },
        {
            gx3::TriangleIndices{0, 2, 1}, gx3::TriangleIndices{0, 3, 2},
            gx3::TriangleIndices{4, 5, 6}, gx3::TriangleIndices{4, 6, 7},
            gx3::TriangleIndices{0, 1, 5}, gx3::TriangleIndices{0, 5, 4},
            gx3::TriangleIndices{1, 2, 6}, gx3::TriangleIndices{1, 6, 5},
            gx3::TriangleIndices{2, 3, 7}, gx3::TriangleIndices{2, 7, 6},
            gx3::TriangleIndices{3, 0, 4}, gx3::TriangleIndices{3, 4, 7}
        }
    };

    const gx3::TriangleBVH bvh = buildTriangleBVH(cube);
    const gx3::Point query{1.7, 0.3, 0.6};
    const auto closest = bvh.closestPoint(query);
    const auto meshBounds = boundingBox(cube);
    const auto candidates = bvh.overlapCandidates(
        gx3::AABB{gx3::Point{0.95, 0.2, 0.2}, gx3::Point{1.05, 0.8, 0.8}}
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
    for (gx3::TriangleId triangleId : candidates)
        std::cout << triangleId << ' ';
    std::cout << '\n';
    return 0;
}
