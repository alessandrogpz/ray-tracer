# Tutorial: Multithreading with OpenMP

Multithreading in this project is optional but highly recommended as it can significantly improve rendering performance. The multithreading is implemented using OpenMP.

## Enabling Multithreading

To enable OpenMP multithreading, you need to make a few minor adjustments to the project configuration and the source code.

### 1. Update `CMakeLists.txt`

Open the `CMakeLists.txt` file in the root of the project. You need to uncomment the `find_package` instruction and the `target_link_libraries` instruction related to OpenMP.

Change to this:
```cmake
# Optional!
# Locates the OpenMP package on the host system to enable CPU multithreading.
# Required for parallelizing our rendering loops (e.g., #pragma omp parallel for).
find_package(OpenMP REQUIRED)

# ... (other cmake code) ...

# Optional!
# Links OpenMP to the core engine.
# Using PUBLIC ensures that any executable linking against raytracer_core
# (like our visualizers and tests) automatically inherits the correct
# compiler and linker flags for OpenMP across all operating systems.
target_link_libraries(raytracer_core PUBLIC OpenMP::OpenMP_CXX)
```

### 2. Update the Rendering Loop
In your main entry Point file (where the main rendering loop occurs, such as `visualizers/SphereShadow.cpp`), you need to uncomment or add an OpenMP pragma directive to tell the compiler to parallelize the loop.

As an example, in `visualizers/SphereShadow.cpp`, uncomment / add the following line just above the outer `for` loop that iterates over the Canvas pixels: `#pragma omp parallel for`
```c++
// Iterate over every row (y) and column (x) of the Canvas
#pragma omp parallel for // Multithreading support enabled
for (size_t y = 0; y < CANVAS_SIZE; ++y)
{
// Compute the world y coordinate (top = +half, bottom = -half)
double world_y = half - pixel_size * y;

    for (size_t x = 0; x < CANVAS_SIZE; ++x)
    {
        // ... rendering logic per pixel ...
    }
}
```
After making these changes, re-run your CMake configuration and build the project.

