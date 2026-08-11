#include "geox/geox.h"
#include <iostream>
#include <string>

namespace gx2 = geox::dim2;
namespace gx3 = geox::dim3;

namespace
{

    gx3::TriangleMesh makeGreatIcosahedron()
    {
        constexpr double goldenRatio = 1.6180339887498948482;

        return gx3::TriangleMesh{
            {
                gx3::Point{-1.0, goldenRatio, 0.0}, gx3::Point{1.0, goldenRatio, 0.0},
                gx3::Point{-1.0, -goldenRatio, 0.0}, gx3::Point{1.0, -goldenRatio, 0.0},
                gx3::Point{0.0, -1.0, goldenRatio}, gx3::Point{0.0, 1.0, goldenRatio},
                gx3::Point{0.0, -1.0, -goldenRatio}, gx3::Point{0.0, 1.0, -goldenRatio},
                gx3::Point{goldenRatio, 0.0, -1.0}, gx3::Point{goldenRatio, 0.0, 1.0},
                gx3::Point{-goldenRatio, 0.0, -1.0}, gx3::Point{-goldenRatio, 0.0, 1.0}
            },
            {
                gx3::TriangleIndices{0, 2, 8}, gx3::TriangleIndices{0, 9, 2},
                gx3::TriangleIndices{0, 4, 6}, gx3::TriangleIndices{0, 8, 4},
                gx3::TriangleIndices{0, 6, 9}, gx3::TriangleIndices{1, 10, 3},
                gx3::TriangleIndices{1, 3, 11}, gx3::TriangleIndices{1, 6, 4},
                gx3::TriangleIndices{1, 4, 10}, gx3::TriangleIndices{1, 11, 6},
                gx3::TriangleIndices{2, 7, 5}, gx3::TriangleIndices{2, 5, 8},
                gx3::TriangleIndices{2, 9, 7}, gx3::TriangleIndices{3, 5, 7},
                gx3::TriangleIndices{3, 10, 5}, gx3::TriangleIndices{3, 7, 11},
                gx3::TriangleIndices{4, 8, 10}, gx3::TriangleIndices{5, 10, 8},
                gx3::TriangleIndices{6, 11, 9}, gx3::TriangleIndices{7, 9, 11}
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

    const gx3::TriangleMesh greatIcosahedron = makeGreatIcosahedron();
    writeTriangleMeshToVtk(greatIcosahedron, outputPath);

    std::cout << "Great icosahedron exported to " << outputPath << "\n";
    std::cout << "Open it in ParaView and select Surface With Edges.\n";
    return 0;
}
