cmake_minimum_required(VERSION 3.28)
project(RayTracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Fetch and build GoogleTest automatically
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

# Prevent GoogleTest from overriding Windows compiler/linker runtime settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# 2. Create a library for your core source modules so they can be reused
add_library(raytracer_core STATIC)
target_sources(raytracer_core
  PUBLIC
    FILE_SET CXX_MODULES FILES
        src/Utils.cppm
        src/Tuple.cppm
        src/Colors.cppm
        src/Canvas.cppm
        src/Matrix.cppm
        src/Transformations.cppm
        src/Ray.cppm
        src/Shapes.cppm
)

# Apply compilation warnings to the core library
if(MSVC)
    target_compile_options(raytracer_core PRIVATE 
        /W4 /permissive- /WX
    )
else()
    target_compile_options(raytracer_core PRIVATE 
        -Wall -Wextra -Wpedantic -Werror
    )
endif()

# 3. Create the Google Tests executable
add_executable(run_tests
        tests/Tuple.t.cpp
        tests/Colors.t.cpp
        tests/Canvas.t.cpp
        tests/MatrixOperations.t.cpp
        tests/MatrixTransformations.t.cpp
        tests/Ray.t.cpp
)
target_link_libraries(run_tests PRIVATE raytracer_core GTest::gtest GTest::gtest_main)

# 4. Create the Visualizer executable
add_executable(visualizers
    visualizers/clock_markers.cpp
)
target_link_libraries(visualizers PRIVATE raytracer_core)

# 5. Create the Freestyle Test executable
add_executable(test_manual
        test_manual.cpp
)
target_link_libraries(test_manual PRIVATE raytracer_core)