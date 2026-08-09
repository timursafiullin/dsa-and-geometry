cmake_minimum_required(VERSION 3.10)

project(dsa-studying VERSION 0.0.1 LANGUAGES CXX)

set(GEOMETRY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/predicates.cpp

    # Primitives
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/primitives/vectors.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/primitives/points.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/primitives/segments.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/primitives/triangles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/primitives/triangle_mesh.cpp

    # Algorithms
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/algorithms/segment2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/algorithms/segment3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/algorithms/triangle2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/algorithms/triangle3.cpp

    # I/O
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/io/vtk.cpp

    # Topology
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/topology/edge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/topology/triangle_topology.cpp
)

add_library(${PROJECT_NAME} ${GEOMETRY_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(great_icosahedron_vtk_demo
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/geometry/great_icosahedron_vtk_demo.cpp
)

target_link_libraries(great_icosahedron_vtk_demo
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
