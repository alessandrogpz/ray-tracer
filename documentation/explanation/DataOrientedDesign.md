# Data-Oriented Design (DOD) in the Ray Tracer

This document outlines the principles of Data-Oriented Design (DOD) as they apply to our Ray Tracer, specifically focusing on CPU cache utilization, memory footprint optimization, and eliminating heap allocation bottlenecks.

---

## 1. Core Principles of DOD

Traditional Object-Oriented Programming (OOP) focuses on encapsulation, objects, and relationships. Data-Oriented Design shifts the focus from **how classes relate** to **how data is processed by the CPU**. 

In high-performance software like a ray tracer, the main bottleneck is rarely CPU instruction count—it is **memory latency** (waiting for data to load from RAM into CPU caches). DOD structures data to:
1. Maximize **spatial cache locality** (loading consecutive data in memory into a single CPU cache line).
2. Minimize the **memory footprint** (fitting more data into L1/L2 caches).
3. Avoid **pointer chasing** (which causes cache misses).
4. Avoid **dynamic heap allocations** (`new`/`malloc`) in hot execution paths.

---

## 2. Case Study: 8-bit Canvas Grid Optimization

### The Problem (OOP Approach)
Originally, each pixel in the canvas was represented as a `Color` object containing four `double` components (Red, Green, Blue, Alpha).
* **Memory Footprint**: 4 channels × 8 bytes (`double`) = **32 bytes per pixel**.
* **Impact**: At 2K resolution ($2560 \times 1440$ pixels), a single canvas occupied **~118 MB** of RAM. This massive footprint meant fewer pixels could fit into the CPU cache at once, degrading cache locality during rendering write-loops and file exporting.

### The DOD Optimization
We introduced `PixelRGBA8`, a packed 8-bit representation of pixel colors, and updated the canvas internal representation:

```cpp
struct PixelRGBA8 {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;
};
```
* **Memory Footprint**: 4 channels × 1 byte (`uint8_t`) = **4 bytes per pixel**.
* **Impact**: The 2K canvas size dropped from **118 MB to 14.7 MB** (an **8x reduction**).
* **API Compatibility**: We kept the public interface (`writePixel` and `pixelAt`) using the HDR `Color` type (double-precision). The conversions (float-to-byte on write, byte-to-float on read) happen transparently within the `Canvas` module:
  - **Write**: `scaleColor` scales/clamps `double` color components to $[0, 255]$ and stores them in `PixelRGBA8`.
  - **Read**: Converts `PixelRGBA8` channels back to `double` values (divided by `255.0`).
* **PPM Export Speedup**: Exporting the canvas to PPM requires integer values ($0-255$). By storing the pixels as `uint8_t` natively, `canvasToPPM()` reads the integer values directly and avoids executing float-to-int conversion loops over millions of pixels, speeding up file output.

---

## 3. Case Study: 100% Pure DOD Shape Architecture

### The Problem (OOP Approach)
Typically in ray tracers, shape polymorphism is achieved via standard object-oriented hierarchies:
* **Storage**: `std::vector<std::unique_ptr<Shape>> shapes`
* **Intersection Loop**: For every ray, the engine iterates over pointers, calls `virtual intersect(...)`, and resolves shape-specific logic dynamically.
* **Impact**: Since shape objects are allocated individually on the heap, traversing the vector triggers **pointer chasing** across memory. Furthermore, virtual function calls inside the intersection loop disrupt instruction pipelines, prevent compiler inlining, and degrade cache locality.

### The DOD Optimization
To maximize CPU cache hits and allow the compiler to fully inline mathematical calculations, we implemented a **100% Pure Data-Oriented Design (DOD)** shape architecture, completely eliminating inheritance, virtual tables (`vtables`), and raw pointers.

1. **Flat, Standalone Structs**:
   We removed the polymorphic `Shape` base class entirely. The `Sphere` struct contains all its data flat and contiguously in memory:
   ```cpp
   struct Sphere {
       Point origin;
       double radius;
       Material material;
       Matrix<4> transform{identity()};
       Matrix<4> transform_inverse{identity()};
       Matrix<4> transform_inverse_transpose{identity()};
       int id;
   };
   ```

2. **Index-Based `Intersection` Records**:
   Instead of storing a raw pointer to a base class (`const Shape* obj`), which forces the CPU to chase pointers during shading, we store the shape's index inside the `World` container's contiguous vector and a type-safe `ShapeType` enum:
   ```cpp
   enum class ShapeType : std::uint8_t {
       Sphere
   };

   struct Intersection {
       double t;
       std::uint32_t shape_index;
       ShapeType shape_type;
   };
   ```
   During shading, the ray tracer queries the contiguous vector in the `World` directly using the index (e.g. `w.spheres[intersection.shape_index]`), ensuring adjacent lookups are served straight from L1/L2 caches.

3. **Compiler Inlining of Shape Logic**:
   With no virtual functions, ray intersections are handled by fast, non-virtual functions:
   ```cpp
   void intersect(const Sphere& s, const Ray& r, std::vector<Intersection>& xs, std::uint32_t index = 0);
   ```
   This allows the C++ compiler to inline the entire quadratic sphere-intersection logic directly inside the main loop, avoiding function call overhead.

4. **Contiguous World Vectors**:
   The `World` struct stores concrete shapes in type-segregated vectors:
   ```cpp
   struct World {
       std::vector<Sphere> spheres;
       PointLight light;
   };
   ```
   When casting rays, the engine loops over each contiguous array sequentially, allowing the CPU hardware pre-fetcher to work at 100% efficiency.

5. **Eliminating Heap Allocations (LocalIntersections & Reused Vectors)**:
   Originally, `local_intersect` returned a `std::vector<double>` and `intersect` returned a `std::vector<Intersection>` per call. For a 2K rendering loop with 3 spheres, this resulted in **~22 million heap allocations and deallocations** per frame. To eliminate this bottleneck:
   * **Stack-Allocated Result Struct**: `local_intersect` returns a stack-allocated, fixed-size `LocalIntersections` struct containing a count and a fixed size double array. This fits in CPU registers and avoids any heap allocation.
   * **Vector Reuse in Hot Loops**: We implemented a high-performance overload `intersect(const Sphere& s, const Ray& r, std::vector<Intersection>& xs, std::uint32_t index = 0)` that appends directly to an existing vector. By instantiating a single vector outside the loop, reserving its maximum possible capacity (6 elements), and calling `xs.clear()` on each pixel iteration, we reduced heap allocations inside the render loop to **zero**.


