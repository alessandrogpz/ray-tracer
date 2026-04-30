# Tutorial: Getting Started with the Ray Tracer

Welcome! This tutorial is designed for new contributors (like our persona, Jamie) and users who want to build the ray tracer for the first time.

By the end of this tutorial, you will have successfully cloned the repository, compiled the project using CMake, and verified your setup by running the test suite.

## Prerequisites

Before we begin, ensure your system has the following tools installed:
- A modern C++ compiler supporting C++23 (e.g., GCC 14+, Clang 17+, MSVC).
- **CMake** (version 3.28 or higher, required for C++ Modules).
- **Ninja** build system (required by CMake for C++ Modules).
- **GoogleTest** (gtest) for building and running the test suite.

> **Note:** If you haven't set up your development environment yet, please refer to our [Environment Setup Guide](../how-to/EnvironmentSetup.md) for detailed, OS-specific installation instructions for these dependencies.

## Step 1: Clone the Repository

First, obtain a copy of the source code. Open your terminal and run:

```bash
git clone <ray_tracer-repository-url> ray-tracer
cd ray-tracer
```

## Step 2: Create a Build Directory

It is a standard C++ practice to build projects in a separate directory to keep the source tree clean. We will create a `build` directory and navigate into it.

```bash
mkdir -p build
cd build
```

## Step 3: Configure the Project with CMake

Now, we instruct CMake to analyze your system, find the necessary dependencies (like GoogleTest), and generate the build files (e.g., Makefiles).

Run the following command from inside the `build` directory:

```bash
cmake -G Ninja ..
```

*CMake reads the `CMakeLists.txt` from the parent directory (`..`) and configures the Ninja build files.*

## Step 4: Build the Project

With the configuration complete, you can compile the ray tracer and the test runner. 

```bash
# Compile using all available CPU cores
cmake --build . -j
```

## Step 5: Run the Test Suite

If the build succeeds, CMake will have generated an executable named `run_tests` in your build directory. This executable contains all the unit tests that verify the ray tracer's math and logic.

Run it using:

```bash
./run_tests
```

### Expected Output

You should see an output from GoogleTest displaying the progress of the tests. If everything is working correctly, the final lines will look like this:

```text
[==========] X tests from Y test suites ran. (Z ms total)
[  PASSED  ] X tests.
```
