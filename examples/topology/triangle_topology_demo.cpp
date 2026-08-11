#include "geox/geox.h"

#include <iostream>

namespace gx2 = geox::dim2;
namespace gx3 = geox::dim3;

int main()
{
    const gx3::TriangleMesh mesh{
        {
            gx3::Point{0.0, 0.0, 0.0}, gx3::Point{1.0, 0.0, 0.0},
            gx3::Point{1.0, 1.0, 0.0}, gx3::Point{0.0, 1.0, 0.0}
        },
        {gx3::TriangleIndices{0, 1, 2}, gx3::TriangleIndices{0, 2, 3}}
    };
    const gx3::TriangleTopology topology = buildTriangleTopology(mesh);
    const gx3::HalfEdgeTopology halfEdges = buildHalfEdgeTopology(mesh, topology);
    const auto loops = extractBoundaryLoops(mesh, halfEdges);

    std::cout << "vertices: " << topology.vertexCount()
              << ", edges: " << topology.edgeCount()
              << ", triangles: " << topology.triangleCount() << '\n';
    std::cout << "manifold: " << std::boolalpha << topology.isManifold()
              << ", closed: " << isClosed(topology)
              << ", boundary loops: " << loops.size() << '\n';
    return 0;
}
