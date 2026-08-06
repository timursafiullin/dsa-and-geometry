cmake_minimum_required(VERSION 3.10)

project(dsa-studying VERSION 0.0.1 LANGUAGES CXX)

set(GEOMETRY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/primitives.cpp
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
