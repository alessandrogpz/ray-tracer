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
- **Implementation details stay private.** Helper types and internal logic live only in the `.cpp` and are invisible to importers.
- **Faster incremental builds.** Changing a function body in the `.cpp` does not invalidate the compiled module interface (BMI), so dependent translation units do not need to recompile.

### Anatomy of a split module

**`src/math/Utils.cppm`** — interface unit:
```cpp
export module rt.utils;          // declares the module and marks it as exportable

export namespace rt {
    constexpr float EPSILON = 0.0001f;
    constexpr float RELATIVE_EPSILON = 0.00001f;

    [[nodiscard]]
    bool equal(float a, float b); // declaration only — no body
}
```

**`src/math/Utils.cpp`** — implementation unit:
```cpp
module rt.utils;                 // attaches this TU to the module (no 'export')

import std;                      // the whole standard library, including <cmath> functions

namespace rt {
    bool equal(const float a, const float b)  // definition
    {
        // Absolute tolerance near zero, relative tolerance for large values
        // (full version and rationale: CoreMath.md, "Comparing Floating-Point Numbers")
        const float difference = std::abs(a - b);
        return difference < EPSILON || difference <= RELATIVE_EPSILON * std::max(std::abs(a), std::abs(b));
    }
}
```

### Rules to follow when adding a new module

1. Create `<Name>.cppm` with `export module rt.<name>;` and only **declarations** (+ `export` on every public symbol).
2. Create `<Name>.cpp` with `module rt.<name>;` (no `export`) and all **definitions**.
3. Use `import std;` for the standard library instead of `#include`. It provides every standard function and type (including `<cmath>`'s `std::sqrt`, `std::pow`, ...), so no global module fragment (`module;` + `#include`) is needed. See [No `#include`s](#no-includes-import-std-only) below.
4. Register both files in `CMakeLists.txt` under `target_sources(raytracer_core ...)` — the `.cppm` with `FILE_SET CXX_MODULES`, the `.cpp` as a regular source.

> **See also:** [C++ Module & Build Architecture](C++Architecture.md) for the CMake configuration that wires this up.

### No `#include`s: `import std` Only

The project uses `import std;` everywhere: modules, implementation units, visualizers, and tests. The only `#include` left is `<gtest/gtest.h>` in the test files, because GoogleTest is not a module and its `TEST`/`EXPECT_*` are macros.

Modules cannot export macros, so standard macros such as `assert` are not available through `import std;`. Use a function instead. `Tuple.cpp`'s private `debugCheck(condition, message)` is the replacement for `assert`:

- It reports the failing file, line, and function via `std::source_location` and calls `std::abort()`.
- It is compiled out when `NDEBUG` is defined (Release builds), just like `assert`. Testing `NDEBUG` is a preprocessor check of a command-line define, not an include.

> [!NOTE]
> Dropping `#include <cmath>` costs no performance: with Release flags, GCC 16 and Clang 23 generate identical machine code for `std::sqrt`/`std::pow` whether they come from `<cmath>` or from `import std;`.

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