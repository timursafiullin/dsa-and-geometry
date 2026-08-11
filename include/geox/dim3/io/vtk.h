#pragma once

#include "geox/dim3/mesh/triangle_mesh.h"

#include <string>

namespace geox::dim3
{

    // Writes mesh as an ASCII legacy VTK POLYDATA file.
    // The resulting file can be opened directly in ParaView.
    void writeTriangleMeshToVtk(
        const TriangleMesh& mesh,
        const std::string& path
    );

} // namespace geox::dim3
