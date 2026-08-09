#include "geox/primitives/triangle_mesh.h"
#include "geox/topology/boundary.h"
#include "geox/topology/half_edge_topology.h"
#include "geox/topology/orientation.h"
#include "geox/topology/triangle_topology.h"

#include <iostream>

using namespace geox;

int main()
{
    const TriangleMesh mesh{
        {
            Point3{0.0, 0.0, 0.0}, Point3{1.0, 0.0, 0.0},
            Point3{1.0, 1.0, 0.0}, Point3{0.0, 1.0, 0.0}
        },
        {TriangleIndices{0, 1, 2}, TriangleIndices{0, 2, 3}}
    };
    const TriangleTopology topology = buildTriangleTopology(mesh);
    const HalfEdgeTopology halfEdges = buildHalfEdgeTopology(mesh, topology);
    const auto loops = extractBoundaryLoops(mesh, halfEdges);

    std::cout << "vertices: " << topology.vertexCount()
              << ", edges: " << topology.edgeCount()
              << ", triangles: " << topology.triangleCount() << '\n';
    std::cout << "manifold: " << std::boolalpha << topology.isManifold()
              << ", closed: " << isClosed(topology)
              << ", boundary loops: " << loops.size() << '\n';
    return 0;
}
