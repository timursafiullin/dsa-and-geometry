#include "test_framework.h"

#include "geox/io/vtk.h"
#include "geox/primitives/triangle_mesh.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

GEOX_TEST(vtk_export_writes_a_legacy_polydata_file)
{
    using namespace geox;

    const TriangleMesh mesh{
        {Point3{0.0, 0.0, 0.0}, Point3{1.0, 0.0, 0.0}, Point3{0.0, 1.0, 0.0}},
        {TriangleIndices{0, 1, 2}}
    };
    const std::string path = std::string(GEOX_TEST_OUTPUT_DIR) + "/triangle.vtk";

    writeTriangleMeshToVtk(mesh, path);

    std::ifstream file(path);
    GEOX_REQUIRE(file.good());
    std::ostringstream contents;
    contents << file.rdbuf();
    const std::string vtk = contents.str();
    GEOX_REQUIRE(vtk.find("# vtk DataFile Version 3.0\n") == 0);
    GEOX_REQUIRE(vtk.find("DATASET POLYDATA\n") != std::string::npos);
    GEOX_REQUIRE(vtk.find("POINTS 3 double\n") != std::string::npos);
    GEOX_REQUIRE(vtk.find("POLYGONS 1 4\n3 0 1 2\n") != std::string::npos);
}

GEOX_TEST(vtk_export_rejects_invalid_meshes_and_unwritable_paths)
{
    using namespace geox;

    const TriangleMesh invalid{{Point3{}}, {TriangleIndices{0, 1, 2}}};
    GEOX_REQUIRE_THROWS_AS(
        writeTriangleMeshToVtk(invalid, std::string(GEOX_TEST_OUTPUT_DIR) + "/invalid.vtk"),
        std::invalid_argument
    );

    const TriangleMesh valid{
        {Point3{}, Point3{1.0, 0.0, 0.0}, Point3{0.0, 1.0, 0.0}},
        {TriangleIndices{0, 1, 2}}
    };
    GEOX_REQUIRE_THROWS_AS(
        writeTriangleMeshToVtk(valid, std::string(GEOX_TEST_OUTPUT_DIR) + "/missing/output.vtk"),
        std::runtime_error
    );
}
