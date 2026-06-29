# Reference: Docstring Style Guide

This project follows a standardized docstring format for documenting functions, structs, and complex logic within the C++ source code. We use a style compatible with Doxygen to ensure that the code is self-documenting and easy to parse by automated tools if needed.

## General Guidelines

*   **Location**: Docstrings belong in the **module interface unit (`.cppm`)**, immediately above the function or struct **declaration**. This is the file that consumers `import`, so it is the canonical place for public-facing documentation.
    *   Do **not** duplicate docstrings in the `.cpp` implementation unit. A brief `// see .cppm for full docs` comment is acceptable there if extra clarity is needed.
    *   For a refresher on why declarations live in `.cppm` and definitions in `.cpp`, see [C++ Implementation Details](C++ImplementationDetails.md#module-interface--implementation-split-cppm--cpp).
*   **Format**: Use the `/** ... */` block comment syntax.
*   **Tags**: Utilize Doxygen-style tags (e.g., `@brief`, `@param`, `@return`, `@note`) to structure the information.
*   **Cross-referencing**: When a function's logic is complex or relies on extensive mathematical theory (like the analytic solution for Ray-Sphere Intersection), use the docstring to briefly explain what the function does, and provide a direct file path reference to the corresponding document in the `documentation/explanation/` or `documentation/references/` folders.

## Example: Function Documentation

Here is an example demonstrating the proper formatting, taken directly from our `intersect` function:

```cpp
/**
 * @brief Calculates the Intersection points of a Ray and a Sphere.
 *
 * Uses the quadratic formula to solve for time `t` where the Ray intersects the Sphere's surface.
 * Math reference: documentation/explanation/RaySphereIntersectionAlgorithm.md
 *
 * @param s The target Sphere geometry.
 * @param r The Ray being cast.
 * @param index The index of the sphere within the container.
 * @return std::vector<Intersection> A collection of Intersection records containing the `t` values, shape_index, and shape_type. Empty if no Intersection.
*/
[[nodiscard]]
std::vector<Intersection> intersect(const Sphere& s, const Ray& r, std::uint32_t index = 0)
{
    // Implementation...
}
```
