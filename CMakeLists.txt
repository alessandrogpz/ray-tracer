cmake_minimum_required(VERSION 3.28)
project(RayTracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional!
# Locates the OpenMP package on the host system to enable CPU multithreading.
# Required for parallelizing our rendering loops (e.g., #pragma omp parallel for).
# find_package(OpenMP REQUIRED)

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
        src/Intersection.cppm
        src/Lights.cppm
        src/Materials.cppm
        src/Shading.cppm
)

# Optional!
# Links OpenMP to the core engine.
# Using PUBLIC ensures that any executable linking against raytracer_core
# (like our visualizers and tests) automatically inherits the correct
# compiler and linker flags for OpenMP across all operating systems.
# target_link_libraries(raytracer_core PUBLIC OpenMP::OpenMP_CXX)

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
        tests/Intersection.t.cpp
        tests/Shapes.t.cpp
        tests/Lights.t.cpp
        tests/Materials.t.cpp
        tests/Shading.t.cpp
)
target_link_libraries(run_tests PRIVATE raytracer_core GTest::gtest GTest::gtest_main)

# 4. (Optional) Create Example Visualizers Executables
add_executable(ClockMarkers
        visualizers/ClockMarkers.cpp
)

add_executable(ProjectTrajectory
        visualizers/ProjectTrajectory.cpp
)

add_executable(SphereShadow
        visualizers/SphereShadow.cpp
)

target_link_libraries(ClockMarkers PRIVATE raytracer_core)
target_link_libraries(ProjectTrajectory PRIVATE raytracer_core)
target_link_libraries(SphereShadow PRIVATE raytracer_core)