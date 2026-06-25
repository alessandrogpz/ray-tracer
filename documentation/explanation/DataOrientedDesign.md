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

## 3. Case Study: DOD Shape Polymorphism

### The Problem (OOP Approach)
Typically in ray tracers, shape polymorphism is achieved via standard object-oriented hierarchies:
* **Storage**: `std::vector<std::unique_ptr<Shape>> shapes`
* **Intersection Loop**: For every ray, the engine iterates over the pointers, calls `virtual intersect(...)`, and resolves shape-specific logic dynamically.
* **Impact**: Since shape objects are allocated individually on the heap, traversing the vector triggers **pointer chasing** across memory. Furthermore, virtual function calls inside the intersection loop disrupt instruction pipelines and degrade cache locality.

### The DOD Optimization
To prepare the ray tracer for type-segregated memory arrays within a scene container (`World`), we implemented a hybrid polymorphism design:

1. **Decoupling from Ray Primitives**:
   We redefined the `Shape` virtual interface to accept raw data types rather than a composite `Ray` object:
   ```cpp
   [[nodiscard]] virtual std::vector<double> local_intersect(Point local_origin, Vector local_direction) const = 0;
   ```
   This broke circular dependencies between C++20 modules (`rt.ray` ➡️ `rt.sphere` ➡️ `rt.intersection` ➡️ `rt.ray`), allowing `rt.shape_base` to remain an independent module.

2. **File & Type Segregation**:
   We renamed `Shapes.cppm` / `Shapes.cpp` to `Sphere.cppm` / `Sphere.cpp` to isolate `Sphere` as its own concrete structure inheriting from `Shape`. Additional shapes (e.g., `Triangle`, `Plane`) will follow this pattern as separate types.
   
3. **Contiguous World Vectors**:
   This architecture enables a flat-memory `World` design:
   ```cpp
   struct World {
       std::vector<Sphere> spheres;
       std::vector<Triangle> triangles;
   };
   ```
   When casting rays, the engine loops over each contiguous array sequentially. No pointer chasing or virtual function resolution occurs during the intersection checks.

4. **APIs Compatibility**:
   We kept `Intersection` records holding a base `const Shape* obj` pointer. Direct properties (like `obj->material`) are accessed immediately from the base class, and virtual dispatch (`normalAt`) is only invoked **once** per ray (during the shading phase for the closest hit), avoiding virtual call overhead on missed rays.

