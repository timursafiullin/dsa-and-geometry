cmake_minimum_required(VERSION 3.15)

project(geox VERSION 1.0.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(GEOX_BUILD_EXAMPLES "Build GEOX examples" ON)
option(GEOX_BUILD_TESTS "Build GEOX tests" ON)

add_library(geox_core INTERFACE)
add_library(geox::core ALIAS geox_core)
set_target_properties(geox_core PROPERTIES EXPORT_NAME core)
target_include_directories(geox_core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(geox_core INTERFACE cxx_std_17)

set(GEOX_DIM2_SOURCES
    src/geox/dim2/predicates.cpp
    src/geox/dim2/primitives/vector.cpp
    src/geox/dim2/primitives/point.cpp
    src/geox/dim2/primitives/segment.cpp
    src/geox/dim2/primitives/triangle.cpp
    src/geox/dim2/algorithms/segment.cpp
    src/geox/dim2/algorithms/triangle.cpp
    src/geox/dim2/spatial/aabb.cpp
    src/geox/dim2/spatial/bounds.cpp
)

add_library(geox_dim2 ${GEOX_DIM2_SOURCES})
add_library(geox::dim2 ALIAS geox_dim2)
set_target_properties(geox_dim2 PROPERTIES
    EXPORT_NAME dim2
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
target_link_libraries(geox_dim2 PUBLIC geox_core)
target_compile_features(geox_dim2 PUBLIC cxx_std_17)

set(GEOX_DIM3_SOURCES
    src/geox/dim3/primitives/vector.cpp
    src/geox/dim3/primitives/point.cpp
    src/geox/dim3/primitives/segment.cpp
    src/geox/dim3/primitives/triangle.cpp
    src/geox/dim3/mesh/triangle_mesh.cpp
    src/geox/dim3/algorithms/segment.cpp
    src/geox/dim3/algorithms/triangle.cpp
    src/geox/dim3/algorithms/triangle_mesh.cpp
    src/geox/dim3/spatial/aabb.cpp
    src/geox/dim3/spatial/bounds.cpp
    src/geox/dim3/spatial/triangle_bvh.cpp
    src/geox/dim3/topology/edge.cpp
    src/geox/dim3/topology/triangle_topology.cpp
    src/geox/dim3/topology/orientation.cpp
    src/geox/dim3/topology/half_edge_topology.cpp
    src/geox/dim3/topology/boundary.cpp
    src/geox/dim3/io/vtk.cpp
)

add_library(geox_dim3 ${GEOX_DIM3_SOURCES})
add_library(geox::dim3 ALIAS geox_dim3)
set_target_properties(geox_dim3 PROPERTIES
    EXPORT_NAME dim3
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
target_link_libraries(geox_dim3 PUBLIC geox_core)
target_compile_features(geox_dim3 PUBLIC cxx_std_17)

add_library(geox_geox INTERFACE)
add_library(geox::geox ALIAS geox_geox)
set_target_properties(geox_geox PROPERTIES EXPORT_NAME geox)
target_link_libraries(geox_geox INTERFACE geox_dim2 geox_dim3)

install(DIRECTORY include/geox DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS geox_core geox_dim2 geox_dim3 geox_geox
    EXPORT geoxTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT geoxTargets
    FILE geoxTargets.cmake
    NAMESPACE geox::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/geox
)

configure_package_config_file(
    cmake/geoxConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/geoxConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/geox
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/geoxConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/geoxConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/geoxConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/geox
)

if(GEOX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GEOX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
