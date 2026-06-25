# Reference: Docstring Style Guide

This project follows a standardized docstring format for documenting functions, structs, and complex logic within the C++ source code. We use a style compatible with Doxygen to ensure that the code is self-documenting and easy to parse by automated tools if needed.

## General Guidelines

*   **Location**: Docstrings should be placed immediately above the function declaration or struct definition.
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
 * Math reference: documentation/explanation/RaySphereIntersection.md
 *
 * @param s The target Sphere geometry.
 * @param r The Ray being cast.
 * @return std::vector<Intersection> A collection of Intersection records containing the `t` values and a pointer to the hit object. Empty if no Intersection.
*/
[[nodiscard]]
std::vector<Intersection> intersect(const Sphere& s, const Ray& r)
{
    // Implementation...
}
```
