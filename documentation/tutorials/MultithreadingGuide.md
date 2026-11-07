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
*   **GCC (the default compiler):** nothing to install. OpenMP support (`libgomp`) ships with GCC itself.
*   **Clang:** install LLVM's OpenMP runtime (`libomp`):
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

The OpenMP configuration inside [CMakeLists.txt](../../CMakeLists.txt) conditionalizes the find package and linkage targets:

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

### 3. The Rendering Loop

You do not need to edit any code: the pixel loop in `render()` (in [`src/scene/Camera.cpp`](../../src/scene/Camera.cpp)) already carries the OpenMP directive, guarded so it only applies when OpenMP is enabled:

```cpp
#ifdef _OPENMP
#pragma omp parallel for collapse(2) schedule(dynamic, 16)
#endif
for (std::size_t y = 0; y < c.vsize; ++y)
{
    for (std::size_t x = 0; x < c.hsize; ++x)
    {
        Ray r = ray_for_pixel(c, x, y);
        Color col = color_at(w, r);
        writePixel(image, x, y, col);
    }
}
```

> [!IMPORTANT]
> **Why this is thread-safe:** each pixel is independent. `color_at()` only *reads* the `World` and keeps all of its working data (the intersection list, the refraction containers, secondary rays) in local variables, so every thread has its own copy. Each thread writes to a different pixel of the `Canvas`.
>
> If you write your own parallel loop, follow the same rule: share nothing that is written to. For example, a `std::vector<Intersection>` reused across pixels must be declared **inside** the parallel loop, not outside it. A vector declared outside is shared by all threads, and their concurrent writes cause a `Segmentation fault (core dumped)`.

When compiled with `ENABLE_OPENMP=OFF`, `_OPENMP` is not defined, the directive disappears, and the same loop runs on a single thread.

---

## 3. Advanced OpenMP Clauses in the Camera Render Loop

In the `rt::render` function (located in [Camera.cpp](../../src/scene/Camera.cpp)), we use a more sophisticated OpenMP pragma directive:

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

