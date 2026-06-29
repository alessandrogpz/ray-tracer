# Reference: C++ Implementation Details

This document outlines the specific C++ conventions and memory management rules for this project.

## Module Interface / Implementation Split (`.cppm` / `.cpp`)

This project uses **C++23 named modules** and strictly separates every module into two files:

| File | Role | Key keyword |
|------|------|-------------|
| `<Name>.cppm` | **Module Interface Unit** — declares the public API | `export module rt.<name>;` |
| `<Name>.cpp`  | **Module Implementation Unit** — defines the function bodies | `module rt.<name>;` (no `export`) |

### Why split them?

Keeping declarations and definitions in separate files mirrors the classical header/source split that C++ developers are familiar with, while fully leveraging the module system:

- **Consumers only see the interface.** Any file that writes `import rt.utils;` compiles against `Utils.cppm` alone — it never touches the `.cpp`.
- **Implementation details stay private.** Helper types, `#include`s for C standard headers (e.g. `#include <cmath>`), and internal logic live only in the `.cpp` and are invisible to importers.
- **Faster incremental builds.** Changing a function body in the `.cpp` does not invalidate the compiled module interface (BMI), so dependent translation units do not need to recompile.

### Anatomy of a split module

**`src/math/Utils.cppm`** — interface unit:
```cpp
export module rt.utils;          // declares the module and marks it as exportable

export namespace rt {
    constexpr float EPSILON = 0.0001f;

    [[nodiscard]]
    bool equal(float a, float b); // declaration only — no body
}
```

**`src/math/Utils.cpp`** — implementation unit:
```cpp
module;                          // global module fragment — legacy #includes go here
#include <cmath>
module rt.utils;                 // attaches this TU to the module (no 'export')

import std;

namespace rt {
    bool equal(const float a, const float b)  // definition
    {
        return std::abs(a - b) < EPSILON;
    }
}
```

### Rules to follow when adding a new module

1. Create `<Name>.cppm` with `export module rt.<name>;` and only **declarations** (+ `export` on every public symbol).
2. Create `<Name>.cpp` with `module rt.<name>;` (no `export`) and all **definitions**.
3. Any C-library `#include` directives belong in the `module;` global fragment at the top of the `.cpp`, never in the `.cppm`.
4. Register both files in `CMakeLists.txt` under `target_sources(raytracer_core ...)` — the `.cppm` with `FILE_SET CXX_MODULES`, the `.cpp` as a regular source.

> **See also:** [C++ Module & Build Architecture](C++Architecture.md) for the CMake configuration that wires this up.

---

## Parameter Passing: Value vs. Reference

When writing mathematical operations or engine logic, follow these strict rules for function signatures based on the byte size of the data structures.

### 1. Pass by Value for Small/Primitive Types (<= 32 Bytes)

Modern 64-bit calling conventions pass small objects (up to 32 bytes, such as 4-element float vectors) directly into CPU registers (such as SSE/AVX/YMM registers). Passing these by value avoids pointer indirection and aliasing issues, allowing the compiler to optimize mathematical operations heavily.

* **Rule:** Pass primitives and structures up to 32 bytes by value.
* **Applies to:** `float`, `int`, `Tuple`, `Point`, `Vector`, `Color`.
* **Example:** `Vector normalizeVector(Vector a)`
* **Example:** `Vector operator+(Vector v1, Vector v2)`

### 2. Pass by Const Reference for Large Types (> 32 Bytes)

Objects larger than 32 bytes cannot fit entirely into the standard fast registers. Passing them by value forces the compiler to copy the entire object onto the stack, which causes performance bottlenecks during heavy iteration (like rendering millions of pixels).

* **Rule:** Pass objects larger than 32 bytes by `const` reference.
* **Applies to:** `Ray` (64 bytes), `Matrix` (128 bytes for 4x4), scene geometry (e.g. `Sphere`), and `Canvas`.
* **Example:** `Point position(const Ray& r, float time)`
* **Example:** `Color pixelAt(const Canvas& c, int x, int y)`

### 3. Pass by Reference for Mutation

If a function explicitly needs to modify an existing variable in the caller's scope, pass by non-const reference.

* **Example:** `void writePixel(Canvas& c, int x, int y, Color pixel_color)`