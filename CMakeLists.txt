cmake_minimum_required(VERSION 3.10)
project(RayTracer)

# Use C++17 or C++20 for modern features
set(CMAKE_CXX_STANDARD 17)

# 1. Find the GTest package you installed with paru
find_package(GTest REQUIRED)

# 2. Include your source directory
include_directories(src)

# 3. Create the test executable
# Add all your .cpp files here as you create them
add_executable(run_tests 
    tests/TupleTests.cpp 
    src/Tuple.cpp
)

# 4. Link GTest and the main() runner provided by GTest
target_link_libraries(run_tests GTest::gtest GTest::gtest_main)