add_executable(geox_tests
    test_main.cpp
    dim2/primitives_test.cpp
    dim2/algorithms_test.cpp
    dim3/spatial_test.cpp
    dim3/topology_test.cpp
    dim3/vtk_test.cpp
    api/api_contract_test.cpp
)

target_include_directories(geox_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(geox_tests PRIVATE geox::geox)
target_compile_definitions(geox_tests PRIVATE
    GEOX_TEST_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}"
)

add_test(NAME geox_tests COMMAND geox_tests)

set(GEOX_PUBLIC_HEADERS
    geox/core/constants.h
    geox/dim2/primitives/vector.h
    geox/dim2/primitives/point.h
    geox/dim2/primitives/segment.h
    geox/dim2/primitives/triangle.h
    geox/dim2/algorithms/segment.h
    geox/dim2/algorithms/triangle.h
    geox/dim2/spatial/aabb.h
    geox/dim2/spatial/bounds.h
    geox/dim2/predicates.h
    geox/dim3/primitives/vector.h
    geox/dim3/primitives/point.h
    geox/dim3/primitives/segment.h
    geox/dim3/primitives/triangle.h
    geox/dim3/algorithms/segment.h
    geox/dim3/algorithms/triangle.h
    geox/dim3/algorithms/triangle_mesh.h
    geox/dim3/mesh/ids.h
    geox/dim3/mesh/triangle_mesh.h
    geox/dim3/topology/edge.h
    geox/dim3/topology/triangle_topology.h
    geox/dim3/topology/half_edge_topology.h
    geox/dim3/topology/boundary.h
    geox/dim3/topology/orientation.h
    geox/dim3/spatial/aabb.h
    geox/dim3/spatial/bounds.h
    geox/dim3/spatial/triangle_bvh.h
    geox/dim3/io/vtk.h
    geox/dim2.h
    geox/dim3.h
    geox/geox.h
)

set(GEOX_HEADER_TEST_SOURCES)

foreach(header IN LISTS GEOX_PUBLIC_HEADERS)
    string(MAKE_C_IDENTIFIER "${header}" header_id)
    set(source "${CMAKE_CURRENT_BINARY_DIR}/header_${header_id}.cpp")
    file(WRITE "${source}" "#include <${header}>\n")
    list(APPEND GEOX_HEADER_TEST_SOURCES "${source}")
endforeach()

add_library(geox_header_self_containment OBJECT ${GEOX_HEADER_TEST_SOURCES})
target_link_libraries(geox_header_self_containment PRIVATE geox::geox)
