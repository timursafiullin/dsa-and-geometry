set(GEOX_EXAMPLE_SOURCES
    primitives/points_demo.cpp
    primitives/segments_demo.cpp
    primitives/triangle_mesh_demo.cpp
    primitives/triangles_demo.cpp
    primitives/vectors_demo.cpp
    topology/triangle_topology_demo.cpp
    vtk/annular_disk_vtk_demo.cpp
    vtk/great_icosahedron_vtk_demo.cpp
)

foreach(example_source IN LISTS GEOX_EXAMPLE_SOURCES)
    get_filename_component(example_name "${example_source}" NAME_WE)

    add_executable(${example_name} "${CMAKE_CURRENT_SOURCE_DIR}/${example_source}")
    target_link_libraries(${example_name} PRIVATE geox)
endforeach()

add_custom_target(generate_great_icosahedron_vtk
    COMMAND $<TARGET_FILE:great_icosahedron_vtk_demo>
            ${CMAKE_BINARY_DIR}/great_icosahedron.vtk
    DEPENDS great_icosahedron_vtk_demo
    COMMENT "Generating great_icosahedron.vtk"
    VERBATIM
)

add_custom_target(generate_annular_disk_vtk
    COMMAND $<TARGET_FILE:annular_disk_vtk_demo>
            ${CMAKE_BINARY_DIR}/annular_disk.vtk
    DEPENDS annular_disk_vtk_demo
    COMMENT "Generating annular disk VTK"
    VERBATIM
)
