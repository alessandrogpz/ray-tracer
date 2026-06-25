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

A prime example of DOD implementation in this codebase is the optimization of the [Canvas](file:///home/aper/Documents/ray-tracer/src/core/Canvas.cppm) grid memory footprint.

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
