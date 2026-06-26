cmake_minimum_required(VERSION 3.28)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "4.3.0")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
else()
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
endif()

project(RayTracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_MODULE_STD ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional!
# Locates the OpenMP package on the host system to enable CPU multithreading.
# Required for parallelizing our rendering loops (e.g., #pragma omp parallel for).
find_package(OpenMP REQUIRED)

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
        src/math/Utils.cppm
        src/math/Tuple.cppm
        src/math/Matrix.cppm
        src/math/Transformations.cppm
        src/core/Colors.cppm
        src/core/Canvas.cppm
        src/scene/ShapeBase.cppm
        src/scene/Sphere.cppm
        src/scene/Ray.cppm
        src/scene/Intersection.cppm
        src/scene/Lights.cppm
        src/scene/Materials.cppm
        src/scene/Shading.cppm
        src/scene/World.cppm
  PRIVATE
        src/math/Utils.cpp
        src/math/Tuple.cpp
        src/math/Transformations.cpp
        src/core/Colors.cpp
        src/core/Canvas.cpp
        src/scene/ShapeBase.cpp
        src/scene/Sphere.cpp
        src/scene/Ray.cpp
        src/scene/Intersection.cpp
        src/scene/Lights.cpp
        src/scene/Materials.cpp
        src/scene/Shading.cpp
        src/scene/World.cpp
)

# Optional!
# Links OpenMP to the core engine.
# Using PUBLIC ensures that any executable linking against raytracer_core
# (like our visualizers and tests) automatically inherits the correct
# compiler and linker flags for OpenMP across all operating systems.
target_link_libraries(raytracer_core PUBLIC OpenMP::OpenMP_CXX)

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
        tests/Sphere.t.cpp
        tests/Lights.t.cpp
        tests/Materials.t.cpp
        tests/Shading.t.cpp
        tests/World.t.cpp
)
target_link_libraries(run_tests PRIVATE raytracer_core GTest::gtest GTest::gtest_main)

# 4. (Optional) Create Example Visualizers Executables
add_executable(1.ProjectTrajectory visualizers/1.ProjectTrajectory.cpp)
add_executable(2.ClockMarkers visualizers/2.ClockMarkers.cpp)
add_executable(3.SphereShadow visualizers/3.SphereShadow.cpp)
add_executable(4.MultipleSphereShadows visualizers/4.MultipleSphereShadows.cpp)
add_executable(5.SpherePhongReflection visualizers/5.SpherePhongReflection.cpp)
add_executable(6.MultipleSpherePhongReflections visualizers/6.MultipleSpherePhongReflections.cpp)

target_link_libraries(1.ProjectTrajectory PRIVATE raytracer_core)
target_link_libraries(2.ClockMarkers PRIVATE raytracer_core)
target_link_libraries(3.SphereShadow PRIVATE raytracer_core)
target_link_libraries(4.MultipleSphereShadows PRIVATE raytracer_core)
target_link_libraries(5.SpherePhongReflection PRIVATE raytracer_core)
target_link_libraries(6.MultipleSpherePhongReflections PRIVATE raytracer_core)

