# Tutorial: Getting Started with the Ray Tracer

By the end of this tutorial, you will have successfully cloned the repository, compiled the project using CMake, and verified your setup by running the test suite.

## Prerequisites

Before we begin, ensure your system has the following tools installed:
- A modern C++ compiler whose standard library ships the C++23 `std` module (GCC 15+, Clang 18.1.2+ with libc++, or MSVC from Visual Studio 2022 17.6+).
- **CMake** (version 3.28 or higher, required for C++ Modules).
- **Ninja** build system (required by CMake for C++ Modules).
- **GoogleTest** (gtest) for building and running the test suite.
- **(Optional) OpenMP** for CPU multithreading (see [Multithreading Guide](../tutorials/MultithreadingGuide.md)).

> **Note:** If you haven't set up your development environment yet, please refer to our [Environment Setup Guide](EnvironmentSetup.md) for detailed, OS-specific installation instructions for these dependencies.

## Step 1: Clone the Repository

First, obtain a copy of the source code. Open your terminal and run:

```bash
git clone <ray_tracer-repository-url> Ray-tracer
cd Ray-tracer
```

## Step 2: Create a Build Directory

It is a standard C++ practice to build projects in a separate directory to keep the source tree clean. We will create a `build` directory and navigate into it.

```bash
mkdir -p build
cd build
```

## Step 3: Configure the Project with CMake

Now, we instruct CMake to analyze your system, find the necessary dependencies (like GoogleTest), and generate the build files (e.g., Ninja files).

Run the following command from inside the `build` directory:

```bash
cmake -G Ninja ..
```

*CMake reads the `CMakeLists.txt` from the parent directory (`..`) and configures the Ninja build files.*

> [!NOTE]
> **OpenMP Multithreading:** By default, OpenMP parallelization is enabled automatically. If you wish to configure and compile a build with multithreading disabled (e.g., if you do not have OpenMP installed), see the **Option B (Without multithreading)** configurations under Step 4 below, or refer to the [Multithreading Guide](../tutorials/MultithreadingGuide.md).

## Step 4: Build the Project & Visualizers

With the configuration complete, you can compile the Ray tracer and the test runner. 

```bash
# Compile using all available CPU cores
cmake --build . -j
```

This command will compile all executables present in the `CMakeLists.txt`.

*(Alternatively, if you know the target name, e.g., `06.FirstScene`, you can build it specifically: `cmake --build . --target 06.FirstScene`)*

### Building & Running Debug vs. Release Configurations

If you want to maintain separate builds for development/debugging and performance benchmarking on the command line, use separate folders. You must specify the **Ninja** generator (`-G Ninja`), which is required for C++23 Modules support.

#### Debug Configuration (No optimization, easy debugging)
```bash
# 1. Configure using Ninja generator
# Option A: With multithreading (Default)
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja

# Option B: Without multithreading
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug -DENABLE_OPENMP=OFF -G Ninja

# 2. Build
cmake --build build-debug

# 3. Enter build-debug Folder
cd build-debug/

# 4. Run the debug binary
./<executable_name>
```

#### Release Configuration (Full optimizations, fast rendering)
```bash
# 1. Configure using Ninja generator
# Option A: With multithreading (Default)
cmake -B build-release -DCMAKE_BUILD_TYPE=Release -G Ninja

# Option B: Without multithreading
cmake -B build-release -DCMAKE_BUILD_TYPE=Release -DENABLE_OPENMP=OFF -G Ninja

# 2. Build
cmake --build build-release

# 3. Enter build-release Folder
cd build-release/

# 4. Run the release binary
./<executable_name>
```

### Available Visualizers
The project currently includes the following visualizers in the `visualizers/` directory, listed in order of complexity. Each one saves a `.ppm` image into the folder it is run from:

1. **`01.ProjectTrajectory`**: Plots a projectile's path under gravity and wind (`OutputProjectTrajectory.ppm`).
2. **`02.ClockMarkers`**: Places twelve clock-hour markers with rotation matrices (`OutputClockMarkers.ppm`).
3. **`03.SphereRayCast`**: Renders the flat silhouette of a sphere, with no shading.
4. **`04.SpherePhongReflection`**: A single sphere with full Phong lighting on a floor plane.
5. **`05.MultipleSpherePhongReflections`**: Three spheres with different materials.
6. **`06.FirstScene`**: A camera scene with shadows, a floor, and two walls (`OutputFirstScene.ppm`).
7. **`07.MatrixTransformations`**: Six spheres, each transformed differently (`OutputMatrixTransformations.ppm`).
8. **`08.StripePattern`**: Stripe patterns: transformed, nested, and perturbed (`OutputStripePattern.ppm`).
9. **`09.ReflectionRefraction`**: Mirrors, glass, and the Fresnel effect (`OutputReflectionRefraction.ppm`).
10. **`99.Showcase`**: One scene combining every feature (`OutputShowcase.ppm`). It is always numbered last.

## Step 5: Run the Test Suite and Visualizers

### Running Tests
If the build succeeds, CMake will have generated an executable named `run_tests` in your build directory. This executable contains all the unit tests that verify the Ray tracer's math and logic.

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
./06.FirstScene
```
Executing a visualizer outputs a `.ppm` [Portable Pixmap](https://en.wikipedia.org/wiki/Netpbm) image file (e.g., `OutputFirstScene.ppm`).

> **Viewing .ppm Files on Windows:**
> Standard Windows applications like "Photos" or "Paint" do not support this format natively. Use one of the following:

*   **[IrfanView](https://www.irfanview.com/)**: A fast, lightweight image viewer that supports `.ppm`.
*   **[GIMP](https://www.gimp.org/)**: A free, open-source editor that supports `.ppm` format natively.
*   **VS Code Extensions**: Search for "PPM Viewer" in the Marketplace to view images directly inside the editor.