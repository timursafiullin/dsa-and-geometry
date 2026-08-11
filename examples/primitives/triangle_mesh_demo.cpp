#include "geox/geox.h"

#include <iostream>

namespace gx2 = geox::dim2;
namespace gx3 = geox::dim3;

int main()
{
    gx3::TriangleMesh square;
    const gx3::VertexId bottomLeft = square.addVertex(gx3::Point{0.0, 0.0, 0.0});
    const gx3::VertexId bottomRight = square.addVertex(gx3::Point{1.0, 0.0, 0.0});
    const gx3::VertexId topRight = square.addVertex(gx3::Point{1.0, 1.0, 0.0});
    const gx3::VertexId topLeft = square.addVertex(gx3::Point{0.0, 1.0, 0.0});

    square.addTriangle({bottomLeft, bottomRight, topRight});
    square.addTriangle({bottomLeft, topRight, topLeft});

    std::cout << "vertices: " << square.vertexCount()
              << ", triangles: " << square.triangleCount() << '\n';
    std::cout << "area: " << square.triangleArea(0) + square.triangleArea(1) << '\n';
    return 0;
}
