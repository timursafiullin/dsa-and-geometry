add_executable(geox_tests
    test_main.cpp
    primitives_test.cpp
    algorithms_test.cpp
    topology_test.cpp
    vtk_test.cpp
)

target_link_libraries(geox_tests PRIVATE geox)
target_compile_definitions(geox_tests PRIVATE
    GEOX_TEST_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}"
)

add_test(NAME geox_tests COMMAND geox_tests)
