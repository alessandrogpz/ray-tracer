# Reference: C++ Implementation Details

This document outlines the specific C++ conventions and memory management rules for this project.

## Parameter Passing: Value vs. Reference

When writing mathematical operations or engine logic, follow these strict rules for function signatures based on the byte size of the data structures.

### 1. Pass by Value for Small/Primitive Types (<= 32 Bytes)

Modern 64-bit calling conventions pass small objects (up to 32 bytes, such as 4-element double vectors) directly into CPU registers (such as SSE/AVX/YMM registers). Passing these by value avoids pointer indirection and aliasing issues, allowing the compiler to optimize mathematical operations heavily.

* **Rule:** Pass primitives and structures up to 32 bytes by value.
* **Applies to:** `double`, `int`, `Tuple`, `Point`, `Vector`, `Color`.
* **Example:** `Vector normalizeVector(Vector a)`
* **Example:** `Vector operator+(Vector v1, Vector v2)`

### 2. Pass by Const Reference for Large Types (> 32 Bytes)

Objects larger than 32 bytes cannot fit entirely into the standard fast registers. Passing them by value forces the compiler to copy the entire object onto the stack, which causes performance bottlenecks during heavy iteration (like rendering millions of pixels).

* **Rule:** Pass objects larger than 32 bytes by `const` reference.
* **Applies to:** `Ray` (64 bytes), `Matrix` (128 bytes for 4x4), scene geometry (e.g. `Sphere`), and `Canvas`.
* **Example:** `Point position(const Ray& r, double time)`
* **Example:** `Color pixelAt(const Canvas& c, int x, int y)`

### 3. Pass by Reference for Mutation

If a function explicitly needs to modify an existing variable in the caller's scope, pass by non-const reference.

* **Example:** `void writePixel(Canvas& c, int x, int y, Color pixel_color)`