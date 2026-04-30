# Reference: Docstring Style Guide

This project follows a standardized docstring format for documenting functions, structs, and complex logic within the C++ source code. We use a style compatible with Doxygen to ensure that the code is self-documenting and easy to parse by automated tools if needed.

## General Guidelines

*   **Location**: Docstrings should be placed immediately above the function declaration or struct definition.
*   **Format**: Use the `/** ... */` block comment syntax.
*   **Tags**: Utilize Doxygen-style tags (e.g., `@brief`, `@param`, `@return`, `@note`) to structure the information.
*   **Cross-referencing**: When a function's logic is complex or relies on extensive mathematical theory (like the analytic solution for ray-sphere intersection), use the docstring to briefly explain what the function does, and provide a direct file path reference to the corresponding document in the `documentation/explanation/` or `documentation/references/` folders.

## Example: Function Documentation

Here is an example demonstrating the proper formatting, taken directly from our `intersect` function:

```cpp
/**
 * @brief Calculates the intersection points of a ray and a sphere.
 * 
 * For a detailed explanation of the analytic solution and the math behind this algorithm, 
 * see: documentation/explanation/RaySphereIntersectionAlgorithm.md
 * 
 * @param s The sphere to test for intersection.
 * @param r The ray being cast.
 * @return std::vector<float> A collection of `t` values where the ray intersects the sphere.
 */
[[nodiscard]]
std::vector<float> intersect(const sphere& s, const ray& r)
{
    // Implementation...
}
```
