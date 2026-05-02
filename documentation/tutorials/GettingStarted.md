# Tutorial: Getting Started with the Ray Tracer

By the end of this tutorial, you will have successfully cloned the repository, compiled the project using CMake, and verified your setup by running the test suite.

## Prerequisites

Before we begin, ensure your system has the following tools installed:
- A modern C++ compiler supporting C++23 (e.g., GCC 14+, Clang 17+, MSVC).
- **CMake** (version 3.28 or higher, required for C++ Modules).
- **Ninja** build system (required by CMake for C++ Modules).
- **GoogleTest** (gtest) for building and running the test suite.
- **(Optional) OpenMP** for CPU multithreading (see [Multithreading Guide](../tutorials/MultithreadingGuide.md)).

> **Note:** If you haven't set up your development environment yet, please refer to our [Environment Setup Guide](EnvironmentSetup.md) for detailed, OS-specific installation instructions for these dependencies.

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

## Step 4: Build the Project & Visualizers

With the configuration complete, you can compile the ray tracer and the test runner. 

```bash
# Compile using all available CPU cores
cmake --build . -j
```

This command will compile all executables present in the `CMakeLists.txt`.

*(Alternatively, if you know the target name, e.g., `SphereShadow`, you can build it specifically: `cmake --build . --target SphereShadow`)*

### Available Visualizers
The project currently includes the following visualizers in the `visualizers/` directory:

1. **`ClockMarkers.cpp`**: Plots twelve points arranged in a circle, demonstrating matrix rotations and translations, saving the result as `OutputClockMarkers.ppm`.
2. **`ProjectTrajectory.cpp`**: Simulates the physics of a projectile affected by gravity and wind, rendering its path as a green curve on a canvas, saving the result as `OutputProjectTrajectory.ppm`.
3. **`SphereShadow.cpp`**: Uses the core ray-tracing engine (rays and intersections) to render 3D spheres on a 2D canvas, demonstrating hit detection, saving the result as `OutputSphereShadow.ppm`.

## Step 5: Run the Test Suite and Visualizers

### Running Tests
If the build succeeds, CMake will have generated an executable named `run_tests` in your build directory. This executable contains all the unit tests that verify the ray tracer's math and logic.

Run it using:

```bash
./run_tests
```

#### Expected Output

You should see an output from GoogleTest displaying the progress of the tests. If everything is working correctly, the final lines will look like this:

```text
[==========] X tests from Y test suites ran. (Z ms total)
[  PASSED  ] X tests.
```

### Running Visualizers
In addition to the tests, you can run the generated visualizer executables from the same build directory. For example:

```bash
./SphereShadow
```
Executing a visualizer outputs a `.ppm` [Portable Pixmap](https://en.wikipedia.org/wiki/Netpbm) image file (e.g., `OutputSphereShadow.ppm`).

> **Viewing .ppm Files on Windows:**
> Standard Windows applications like "Photos" or "Paint" do not support this format natively. Use one of the following:

*   **[IrfanView](https://www.irfanview.com/)**: A fast, lightweight image viewer that supports `.ppm`.
*   **[GIMP](https://www.gimp.org/)**: A free, open-source editor that supports `.ppm` format natively.
*   **VS Code Extensions**: Search for "PPM Viewer" in the Marketplace to view images directly inside the editor.