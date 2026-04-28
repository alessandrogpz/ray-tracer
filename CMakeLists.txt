cmake_minimum_required(VERSION 3.28)
project(RayTracer)

# C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Find the GTest package
find_package(GTest REQUIRED)

# 2. Create the test executable
add_executable(run_tests 
    tests/TupleTests.cpp 
    tests/ColorsTests.cpp
    tests/CanvasTests.cpp
    tests/MatrixOperationsTests.cpp
    tests/MatrixTransformationsTests.cpp
)

# 3. Add C++ Modules
target_sources(run_tests
  PUBLIC
    FILE_SET CXX_MODULES FILES
      src/Utils.cppm
      src/Tuple.cppm
      src/Colors.cppm
      src/Canvas.cppm
      src/Matrix.cppm
      src/Transformations.cppm
)

if(MSVC)
    target_compile_options(run_tests PRIVATE 
        /W4           # High warning level
        /permissive-  # Enforce standard conformance
        /WX           # Treat warnings as errors
    )
else()
    target_compile_options(run_tests PRIVATE 
        -Wall                   # Enable most warnings
        -Wextra                 # Enable even more warnings
        -Wpedantic              # Enforce strict ISO C++ compliance
        -Werror                 # Treat warnings as errors
    )
endif()

# 4. Link GTest and the main() runner provided by GTest
target_link_libraries(run_tests GTest::gtest GTest::gtest_main)