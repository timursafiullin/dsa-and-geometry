#include "geox/dim3/io/vtk.h"
#include "geox/dim3/mesh/triangle_mesh.h"

#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace geox::dim3
{

    void writeTriangleMeshToVtk(
        const TriangleMesh& mesh,
        const std::string& path
    )
    {
        if (!mesh.isValid())
            throw std::invalid_argument("Cannot export an invalid triangle mesh.");

        std::ofstream output(path);
        if (!output)
            throw std::runtime_error("Cannot open VTK file for writing: " + path);

        output << "# vtk DataFile Version 3.0\n";
        output << "Triangle mesh\n";
        output << "ASCII\n";
        output << "DATASET POLYDATA\n";

        output << std::setprecision(17);
        output << "POINTS " << mesh.vertexCount() << " double\n";
        for (const Point& vertex : mesh.vertices())
            output << vertex.x() << ' ' << vertex.y() << ' ' << vertex.z() << '\n';

        output << "POLYGONS " << mesh.triangleCount()
               << ' ' << mesh.triangleCount() * 4 << '\n';
        for (const TriangleIndices& triangle : mesh.triangles())
            output << "3 "
                   << triangle[0] << ' '
                   << triangle[1] << ' '
                   << triangle[2] << '\n';

        if (!output)
            throw std::runtime_error("Failed to write VTK file: " + path);
    }

} // namespace geox::dim3
