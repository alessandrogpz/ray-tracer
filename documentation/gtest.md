# Ray Tracer Challenge: C++ Development Environment

This project uses **GoogleTest (gTest)** for unit testing and **CMake** as the build system. This guide covers the setup and workflow specifically for **CachyOS/Arch Linux**.

## 1. Installation

On CachyOS, we use `pacman` to install the base development tools and the GTest source.

```bash
# Install base development tools, CMake, and GoogleTest
sudo pacman -S base-devel cmake gtest

```

> **Note:** Arch-based systems install GTest source files to `/usr/src/gtest`. Modern CMake will find these automatically.

---

## 2. Project Structure

Ensure your project follows this layout to keep your source code and tests separated:

```text
/RayTracer
├── CMakeLists.txt         # Build configuration
├── README.md              # This file
├── /src                   # Your Ray Tracer logic
│   ├── tuple.h
│   └── tuple.cpp
└── /tests                 # Your Unit Tests
    └── test_tuples.cpp

```

---

## 3. Configuration (`CMakeLists.txt`)

Your `CMakeLists.txt` should look like this to properly link the system-installed GTest:

```cmake
cmake_minimum_required(VERSION 3.10)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

# 1. Locate GTest on the system
find_package(GTest REQUIRED)

# 2. Add your executable
# We include src/tuple.cpp so the tests can access your functions
add_executable(run_tests
    tests/test_tuples.cpp
    src/tuple.cpp
)

# 3. Link GTest
# GTest::gtest_main automatically provides a main() function for you
target_link_libraries(run_tests GTest::gtest GTest::gtest_main)

```

---

## 4. Writing a Test

In `tests/test_tuples.cpp`, use the `TEST()` macro. **Do not write a `main()` function** in your test files; GoogleTest provides one via the linker.

```cpp
#include <gtest/gtest.h>
#include "../src/tuple.h"

TEST(TupleTest, IsAPoint) {
    tuple p = createPoint(4.3, -4.2, 3.1);
    EXPECT_FLOAT_EQ(p.w, 1.0);
}

```

---

## 5. How to Build and Run

Follow these steps in your terminal to compile and execute your tests:

```bash

# 0. (optional) Delete previous build
rm -rf build

# 1. Create a build directory (keeps project clean)
mkdir -p build && cd build

# 2. Configure with CMake
	# Home
	cmake ..
	# 42
	cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/gtest_install

# 3. Build the project (using all CPU cores)
make -j$(nproc)

# 4. Run the tests
./run_tests

```

### Expected Result

A successful run will show a green summary:

- `[  PASSED  ] X tests.`
- If you see just numbers (like `1` and `0`), check that you aren't using your own `main()` function instead of GoogleTest's.

---

## 6. Ray Tracer Pro-Tips

- **Float Comparisons:** Use `EXPECT_FLOAT_EQ(val1, val2)` for simple checks.
- **Epsilon/Margin of Error:** Use `EXPECT_NEAR(val1, val2, 0.0001)` when checking complex math or matrix transformations to account for rounding errors.

---
