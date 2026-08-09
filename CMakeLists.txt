cmake_minimum_required(VERSION 3.10)

project(geox VERSION 0.2.0 LANGUAGES CXX)

option(GEOX_BUILD_EXAMPLES "Build GEOX examples" ON)
option(GEOX_BUILD_TESTS "Build GEOX tests" ON)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/algorithms/triangle_mesh.cpp

    # Spatial
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/spatial/aabb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/spatial/bounds.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geox/spatial/triangle_bvh.cpp

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

if(GEOX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GEOX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
