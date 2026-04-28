cmake_minimum_required(VERSION 3.28)
project(RayTracer)

# C++20
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Find the GTest package
find_package(GTest REQUIRED)

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
    tests/TupleTests.cpp 
    tests/ColorsTests.cpp
    tests/CanvasTests.cpp
    tests/MatrixOperationsTests.cpp
    tests/MatrixTransformationsTests.cpp
)
target_link_libraries(run_tests PRIVATE raytracer_core GTest::gtest GTest::gtest_main)

# 4. Create the Visualizer executable
add_executable(visualizers
    visualizers/project_trajectory.cpp
)
target_link_libraries(visualizers PRIVATE raytracer_core)

# 5. Create the Freestyle Test executable
add_executable(test_manual
        test_manual.cpp
)
target_link_libraries(test_manual PRIVATE raytracer_core)