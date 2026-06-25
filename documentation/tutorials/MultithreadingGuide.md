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
In your main entry Point file (where the main rendering loop occurs, such as `visualizers/6.MultipleSpherePhongReflections.cpp`), you need to uncomment or add the OpenMP pragma directive just above the outer `for` loop that iterates over the rows (`y`).

> [!IMPORTANT]
> **Avoid Thread Data Races (Segmentation Faults):** 
> To maximize rendering performance, the engine uses a pre-allocated vector to collect intersections. If you parallelize the rendering loop, you **MUST** declare the `std::vector<Intersection> xs` variable **inside** the outer parallelized `y` loop (and NOT outside it).
> 
> Declaring `xs` outside the loop makes it shared across all threads, leading to concurrent writes and causing a `Segmentation fault (core dumped)` at runtime. Declaring it inside the outer loop makes it thread-local, which is safe and runs at maximum speed (only 1 allocation per thread per row).

Example rendering loop with multithreading enabled:
```c++
    #pragma omp parallel for // Multithreading support enabled
    // Iterate over every row (y) and column (x) of the Canvas
    for (std::size_t y = 0; y < CANVAS_HEIGHT; ++y)
    {
        // Compute the world y coordinate
        double world_y = half_height - pixel_size * y;

        // Declare xs INSIDE the parallelized loop so it is thread-local!
        std::vector<Intersection> xs;
        xs.reserve(6); 

        for (std::size_t x = 0; x < CANVAS_WIDTH; ++x)
        {
            // Calculate ray, clear local vector, and intersect
            xs.clear();
            intersect(s1, r, xs);
            intersect(s2, r, xs);
            // ...
        }
    }
```
After making these changes, re-run your CMake configuration and build the project.
