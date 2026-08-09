#include "geox/primitives/triangle_mesh.h"

#include <iostream>
#include <vector>

using namespace geox;

void printMesh(const TriangleMesh& mesh)
{
    std::cout << "Vertices:\n";
    for (std::size_t index = 0; index < mesh.vertices().size(); ++index)
        std::cout << "  " << index << ": " << mesh.vertices()[index] << '\n';

    std::cout << "Triangles:\n";
    for (std::size_t index = 0; index < mesh.triangles().size(); ++index)
    {
        const TriangleIndices& indices = mesh.triangles()[index];
        std::cout << "  " << index << ": ("
                  << indices[0] << ", "
                  << indices[1] << ", "
                  << indices[2] << ")\n";
    }
}

int main()
{
    std::cout << "=== GEOX triangle mesh demo ===\n\n";

    TriangleMesh square;
    VertexId bottomLeft = square.addVertex(Point3{0.0, 0.0, 0.0});
    VertexId bottomRight = square.addVertex(Point3{1.0, 0.0, 0.0});
    VertexId topRight = square.addVertex(Point3{1.0, 1.0, 0.0});
    VertexId topLeft = square.addVertex(Point3{0.0, 1.0, 0.0});

    square.addTriangle({bottomLeft, bottomRight, topRight});
    square.addTriangle({bottomLeft, topRight, topLeft});

    std::cout << "A square split into two triangles\n";
    printMesh(square);
    std::cout << "  valid = " << std::boolalpha << square.isValid() << '\n';
    std::cout << "  geometrically valid = " << square.isGeometricallyValid() << '\n';

    double totalArea = 0.0;
    for (std::size_t index = 0; index < square.triangleCount(); ++index)
    {
        double area = square.triangleArea(index);
        totalArea += area;

        std::cout << "  triangle " << index
                  << ": area = " << area
                  << ", normal = " << square.triangleNormal(index) << '\n';
    }
    std::cout << "  total area = " << totalArea << "\n\n";

    TriangleMesh invalidIndices{
        std::vector<Point3>{Point3{0.0, 0.0, 0.0}},
        std::vector<TriangleIndices>{TriangleIndices{0, 1, 2}}
    };
    std::cout << "Triangle with unavailable vertex indices is valid = "
              << invalidIndices.isValid() << '\n';

    TriangleMesh degenerate;
    VertexId first = degenerate.addVertex(Point3{0.0, 0.0, 0.0});
    VertexId second = degenerate.addVertex(Point3{1.0, 0.0, 0.0});
    VertexId third = degenerate.addVertex(Point3{2.0, 0.0, 0.0});
    degenerate.addTriangle({first, second, third});

    std::cout << "Collinear triangle is valid = " << degenerate.isValid() << '\n';
    std::cout << "Collinear triangle is geometrically valid = "
              << degenerate.isGeometricallyValid() << '\n';

    return 0;
}
