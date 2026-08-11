#include "geox/geox.h"

#include <cmath>
#include <iostream>
#include <string>

namespace gx2 = geox::dim2;
namespace gx3 = geox::dim3;

namespace
{
    constexpr std::size_t kSegments = 16;
    constexpr double kOuterRadius = 2.0;
    constexpr double kInnerRadius = 0.75;
    constexpr double kPi = 3.14159265358979323846;

    gx3::TriangleMesh makeAnnularDisk()
    {
        gx3::TriangleMesh mesh;

        for (std::size_t index = 0; index < kSegments; ++index)
        {
            const double angle = 2.0 * kPi * index / kSegments;
            const double x = std::cos(angle);
            const double y = std::sin(angle);
            mesh.addVertex(gx3::Point{kOuterRadius * x, kOuterRadius * y, 0.0});
        }

        for (std::size_t index = 0; index < kSegments; ++index)
        {
            const double angle = 2.0 * kPi * index / kSegments;
            const double x = std::cos(angle);
            const double y = std::sin(angle);
            mesh.addVertex(gx3::Point{kInnerRadius * x, kInnerRadius * y, 0.0});
        }

        for (std::size_t index = 0; index < kSegments; ++index)
        {
            const gx3::VertexId outer = index;
            const gx3::VertexId nextOuter = (index + 1) % kSegments;
            const gx3::VertexId inner = kSegments + index;
            const gx3::VertexId nextInner = kSegments + (index + 1) % kSegments;
            mesh.addTriangle({outer, nextOuter, nextInner});
            mesh.addTriangle({outer, nextInner, inner});
        }

        return mesh;
    }
} // namespace

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " [output-file.vtk]\n";
        return 1;
    }

    const std::string outputPath = argc == 2 ? argv[1] : "annular_disk.vtk";
    writeTriangleMeshToVtk(makeAnnularDisk(), outputPath);
    std::cout << "Annular disk exported to " << outputPath << '\n';
    return 0;
}
