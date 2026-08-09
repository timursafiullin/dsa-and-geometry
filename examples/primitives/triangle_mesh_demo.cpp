#include "geox/primitives/triangle_mesh.h"

#include <iostream>

using namespace geox;

int main()
{
    TriangleMesh square;
    const VertexId bottomLeft = square.addVertex(Point3{0.0, 0.0, 0.0});
    const VertexId bottomRight = square.addVertex(Point3{1.0, 0.0, 0.0});
    const VertexId topRight = square.addVertex(Point3{1.0, 1.0, 0.0});
    const VertexId topLeft = square.addVertex(Point3{0.0, 1.0, 0.0});

    square.addTriangle({bottomLeft, bottomRight, topRight});
    square.addTriangle({bottomLeft, topRight, topLeft});

    std::cout << "vertices: " << square.vertexCount()
              << ", triangles: " << square.triangleCount() << '\n';
    std::cout << "area: " << square.triangleArea(0) + square.triangleArea(1) << '\n';
    return 0;
}
