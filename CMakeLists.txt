cmake_minimum_required(VERSION 3.10)

project(geox VERSION 0.1.0 LANGUAGES CXX)

set(GEOX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/predicates.cpp

    # Primitives
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/primitives/vectors.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/primitives/points.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/primitives/segments.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/primitives/triangles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/primitives/triangle_mesh.cpp

    # Algorithms
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/algorithms/segment2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/algorithms/segment3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/algorithms/triangle2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/algorithms/triangle3.cpp

    # I/O
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/io/vtk.cpp

    # Topology
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/topology/edge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/topology/triangle_topology.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/topology/orientation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/topology/half_edge_topology.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/topology/boundary.cpp
)

add_library(${PROJECT_NAME} ${GEOX_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_executable(great_icosahedron_vtk_demo
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/vtk/great_icosahedron_vtk_demo.cpp
)

target_link_libraries(great_icosahedron_vtk_demo
    PRIVATE
        ${PROJECT_NAME}
)

add_executable(annular_disk_vtk_demo
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/vtk/annular_disk_vtk_demo.cpp
)

target_link_libraries(annular_disk_vtk_demo
    PRIVATE
        ${PROJECT_NAME}
)

add_custom_target(generate_great_icosahedron_vtk
    COMMAND $<TARGET_FILE:great_icosahedron_vtk_demo>
            ${CMAKE_CURRENT_BINARY_DIR}/great_icosahedron.vtk
    DEPENDS great_icosahedron_vtk_demo
    COMMENT "Generating great_icosahedron.vtk"
    VERBATIM
)

add_custom_target(generate_annular_disk_vtk
    COMMAND $<TARGET_FILE:annular_disk_vtk_demo>
            ${CMAKE_CURRENT_BINARY_DIR}/annular_disk.vtk
    DEPENDS annular_disk_vtk_demo
    COMMENT "Generating annular disk VTK"
    VERBATIM
)
