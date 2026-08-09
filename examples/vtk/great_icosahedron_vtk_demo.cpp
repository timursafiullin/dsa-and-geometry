#include "geox/io/vtk.h"
#include "geox/primitives/triangle_mesh.h"
#include <iostream>
#include <string>

using namespace geox;

namespace
{

    TriangleMesh makeGreatIcosahedron()
    {
        constexpr double goldenRatio = 1.6180339887498948482;

        return TriangleMesh{
            {
                Point3{-1.0, goldenRatio, 0.0}, Point3{1.0, goldenRatio, 0.0},
                Point3{-1.0, -goldenRatio, 0.0}, Point3{1.0, -goldenRatio, 0.0},
                Point3{0.0, -1.0, goldenRatio}, Point3{0.0, 1.0, goldenRatio},
                Point3{0.0, -1.0, -goldenRatio}, Point3{0.0, 1.0, -goldenRatio},
                Point3{goldenRatio, 0.0, -1.0}, Point3{goldenRatio, 0.0, 1.0},
                Point3{-goldenRatio, 0.0, -1.0}, Point3{-goldenRatio, 0.0, 1.0}
            },
            {
                TriangleIndices{0, 2, 8}, TriangleIndices{0, 9, 2},
                TriangleIndices{0, 4, 6}, TriangleIndices{0, 8, 4},
                TriangleIndices{0, 6, 9}, TriangleIndices{1, 10, 3},
                TriangleIndices{1, 3, 11}, TriangleIndices{1, 6, 4},
                TriangleIndices{1, 4, 10}, TriangleIndices{1, 11, 6},
                TriangleIndices{2, 7, 5}, TriangleIndices{2, 5, 8},
                TriangleIndices{2, 9, 7}, TriangleIndices{3, 5, 7},
                TriangleIndices{3, 10, 5}, TriangleIndices{3, 7, 11},
                TriangleIndices{4, 8, 10}, TriangleIndices{5, 10, 8},
                TriangleIndices{6, 11, 9}, TriangleIndices{7, 9, 11}
            }
        };
    }

} // namespace

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " [output-file.vtk]\n";
        return 1;
    }

    const std::string outputPath = argc == 2
        ? argv[1]
        : "great_icosahedron.vtk";

    const TriangleMesh greatIcosahedron = makeGreatIcosahedron();
    writeTriangleMeshToVtk(greatIcosahedron, outputPath);

    std::cout << "Great icosahedron exported to " << outputPath << "\n";
    std::cout << "Open it in ParaView and select Surface With Edges.\n";
    return 0;
}
