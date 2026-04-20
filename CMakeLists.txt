cmake_minimum_required(VERSION 3.10)
project(RayTracer)

# C++20
set(CMAKE_CXX_STANDARD 20)

# 1. Find the GTest package
find_package(GTest REQUIRED)

# 2. Include your source directory
include_directories(src)

# 3. Create the test executable
add_executable(run_tests 
    tests/TupleTests.cpp 
    tests/ColorsTests.cpp
    tests/CanvasTests.cpp
    tests/MatrixOperationsTests.cpp
    tests/MatrixTransformationsTests.cpp
    src/Tuple.cpp
    src/Colors.cpp
    src/Canvas.cpp
    src/Matrix.cpp
    src/Utils.cpp
    src/Transformations.cpp
)

if(MSVC)
    target_compile_options(run_tests PRIVATE 
        /W4           # High warning level
        /permissive-  # Enforce standard conformance (disables non-standard MSVC behavior)
        /w14242       # 'identifier': conversion from 'type1' to 'type1', possible loss of data
        /w14265       # 'class': class has virtual functions, but destructor is not virtual
        /w14287       # 'operator': unsigned/negative constant mismatch
        /w14296       # 'operator': expression is always false
        /w14311       # 'variable' : pointer truncation from 'type' to 'type'
        /w14826       # Conversion from 'type1' to 'type2' is sign-extended
        /WX           # Treat warnings as errors
    )
else()
    target_compile_options(run_tests PRIVATE 
        -Wall                   # Enable most warnings
        -Wextra                 # Enable even more warnings
        -Wpedantic              # Enforce strict ISO C++ compliance
        -Wshadow                # Warn if a local variable shadows another
        -Wnon-virtual-dtor      # Warn if a class with virtual functions has a non-virtual destructor
        -Wold-style-cast        # Warn if a C-style cast is used
        -Wcast-align            # Warn about pointer casts which increase alignment
        -Wunused                # Warn about anything unused
        -Woverloaded-virtual    # Warn if a derived function hides a virtual function
        -Wconversion            # Warn for implicit type conversions (what you saw on Windows)
        -Wsign-conversion       # Warn for implicit conversions between signed and unsigned
        -Wdouble-promotion      # Warn if a float is implicitly promoted to double
        -Wformat=2              # Strict check for printf/scanf-like functions
        -Werror                 # Treat warnings as errors
    )
endif()

# 4. Link GTest and the main() runner provided by GTest
target_link_libraries(run_tests GTest::gtest GTest::gtest_main)