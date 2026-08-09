#pragma once

#include <string>

namespace geox
{

    class TriangleMesh;

    // Writes mesh as an ASCII legacy VTK POLYDATA file.
    // The resulting file can be opened directly in ParaView.
    void writeTriangleMeshToVtk(
        const TriangleMesh& mesh,
        const std::string& path
    );

} // namespace geox
