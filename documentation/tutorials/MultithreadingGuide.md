# Tutorial: Multithreading with OpenMP

Multithreading in this project is optional but highly recommended as it can significantly improve rendering performance. The multithreading is implemented using OpenMP.

## 1. Installing OpenMP

Before enabling OpenMP in your build, make sure you have the OpenMP library installed on your system:

### macOS
Apple's default compiler (Apple Clang) does not support OpenMP out of the box. Install the official LLVM toolchain and the OpenMP runtime library via Homebrew:
```bash
brew install llvm libomp
```
*(Make sure to export the LLVM path as described in the [Environment Setup Guide](EnvironmentSetup.md)).*

### Linux
*   **Ubuntu / Debian:**
    ```bash
    sudo apt update && sudo apt install libomp-dev
    ```
*   **Fedora:**
    ```bash
    sudo dnf install libomp
    ```
*   **Arch Linux:**
    ```bash
    sudo pacman -S openmp
    ```

### Windows
No separate installation is required. OpenMP is natively supported by the MSVC compiler included with **Visual Studio 2022**.

---

## 2. Enabling and Disabling Multithreading

Multithreading is optional and can be toggled on or off at configuration time using the `ENABLE_OPENMP` CMake option.

### 1. Toggle via CMake CLI

To compile the project with or without OpenMP support:

*   **Turn Multithreading ON (Default):**
    ```bash
    cmake -B build-debug -DENABLE_OPENMP=ON -G Ninja
    cmake --build build-debug
    ```
*   **Turn Multithreading OFF:**
    ```bash
    cmake -B build-debug -DENABLE_OPENMP=OFF -G Ninja
    cmake --build build-debug
    ```

When compiled with `ENABLE_OPENMP=OFF`, the compiler does not include OpenMP flags and libraries, meaning all `#pragma omp` directives are safely ignored and the code executes sequentially on a single thread.

### 2. Under the Hood: `CMakeLists.txt`

The OpenMP configuration inside [CMakeLists.txt](file:///home/aper/Documents/ray-tracer/CMakeLists.txt) conditionalizes the find package and linkage targets:

```cmake
option(ENABLE_OPENMP "Enable OpenMP CPU parallelization" ON)
if(ENABLE_OPENMP)
    find_package(OpenMP REQUIRED)
endif()

# ... (target source definitions) ...

if(ENABLE_OPENMP)
    target_link_libraries(raytracer_core PUBLIC OpenMP::OpenMP_CXX)
endif()
```

### 3. Update the Rendering Loop
In your main entry Point file (where the main rendering loop occurs, such as `visualizers/7.MultipleSpherePhongReflections.cpp`), you need to uncomment or add the OpenMP pragma directive just above the outer `for` loop that iterates over the rows (`y`).

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
        float world_y = half_height - pixel_size * y;

        // Declare xs INSIDE the parallelized loop so it is thread-local!
        std::vector<Intersection> xs;
        xs.reserve(6); 

        for (std::size_t x = 0; x < CANVAS_WIDTH; ++x)
        {
            // Calculate ray, clear local vector, and intersect
            xs.clear();
            intersect(s1, r, xs, 0);
            intersect(s2, r, xs, 1);
            
            // If a valid hit occurs, calculate lighting and Color the pixel
            if (auto h = hit(xs))
            {
                const Sphere& hit_sphere = (h->shape_index == 0) ? s1 : s2;
                Point p = position(r, h->t);
                Vector normal = normalAt(hit_sphere, p);
                Vector eye = -r.direction;

                Color pixel_color = lighting(hit_sphere.material, light, p, eye, normal);
                writePixel(c, x, y, pixel_color);
            }
        }
    }
```
After making these changes, re-run your CMake configuration and build the project.

---

## 3. Advanced OpenMP Clauses in the Camera Render Loop

In the `rt::render` function (located in [Camera.cpp](file:///file:///home/aper/Documents/ray-tracer/src/scene/Camera.cpp)), we use a more sophisticated OpenMP pragma directive:

```cpp
#pragma omp parallel for collapse(2) schedule(dynamic, 16)
for (std::size_t y = 0; y < c.vsize; ++y)
{
    for (std::size_t x = 0; x < c.hsize; ++x)
    {
        // Rendering logic...
    }
}
```

Here is a breakdown of what these clauses accomplish and why they are selected for the ray tracer:

### 1. `collapse(2)`
*   **What it does:** Combines (collapses) the nested loops (the outer loop `y` and inner loop `x`) into a single, flat loop of size `vsize * hsize` (e.g., $3,686,400$ iterations at 1440p).
*   **Why we use it:** By default, OpenMP only parallelizes the outermost loop (`y`). On high-core-count processors, parallelizing only the vertical rows might not provide enough distinct work chunks to keep all cores busy. Collapsing the loops provides millions of parallel tasks, eliminating thread starvation and allowing the workload to scale cleanly across massive thread pools.

### 2. `schedule(dynamic, 16)`
*   **What it does:** Distributes the flat loop iterations dynamically at runtime. Threads fetch a chunk of `16` loop iterations, process them, and request another chunk from the scheduler when idle.
*   **Why we use it:**
    *   **Imbalance Prevention:** In a ray tracer, computing pixel colors is not uniform. A ray that misses all geometry resolves instantly to the background color. A ray that hits a sphere must compute surface normals, reflections, lighting calculations, and cast shadow rays.
    *   If we used `static` scheduling (equal pre-allocated division), threads assigned to the "empty background" sections would finish early and sit idle while threads rendering the complex spheres would struggle. `dynamic` scheduling ensures that threads are continuously fed work as they become available.
    *   **Chunk Size (`16`):** Handing out single iterations (`chunk_size=1`) introduces scheduler overhead. Specifying `16` iterations per request amortizes scheduling overhead while maintaining fine-grained load balancing.

