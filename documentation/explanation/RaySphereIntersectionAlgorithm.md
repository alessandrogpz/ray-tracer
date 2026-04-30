# Explanation: Ray-Sphere Intersection Algorithm

The `intersect(sphere, ray)` function determines the points where a ray intersects a sphere. The approach used is an **analytic solution**, relying on algebraic equations rather than numerical approximation.

## The Math Behind the Intersection

To find where a ray and a sphere intersect, we need to find the points in 3D space that satisfy both the equation of the ray and the equation of the sphere.

### 1. The Equation of a Ray

A ray is defined by an origin point ($O$) and a direction vector ($D$). Any point ($P$) on the ray can be found by traveling a certain distance ($t$) along its direction:

$$ P = O + tD $$

### 2. The Equation of a Sphere

A sphere is defined by its center point ($C$) and its radius ($r$). Any point ($P$) lies on the surface of the sphere if the distance from the center to that point is exactly equal to the radius. Using the Pythagorean theorem (or dot product), this is expressed as:

$$ (P - C) \cdot (P - C) = r^2 $$

For our simplified case, the sphere is at the world origin ($C = (0, 0, 0)$) and is a unit sphere ($r = 1$). 

### 3. Combining the Equations

To find the intersection, we substitute the ray equation into the sphere equation. This means we are looking for a distance ($t$) where the resulting point $P$ is on the sphere.

Substitute $P = O + tD$ into $(P - C) \cdot (P - C) = r^2$:

$$ (O + tD - C) \cdot (O + tD - C) = r^2 $$

Let the vector from the sphere's center to the ray's origin be $\vec{V} = O - C$. In our code, this is `sphere_to_ray`. 
The equation simplifies to:

$$ (tD + \vec{V}) \cdot (tD + \vec{V}) = r^2 $$

Expanding the dot product gives a quadratic equation in terms of $t$:

$$ (D \cdot D)t^2 + 2(D \cdot \vec{V})t + (\vec{V} \cdot \vec{V} - r^2) = 0 $$

### 4. Solving the Quadratic Equation

This matches the standard quadratic form:

$$ at^2 + bt + c = 0 $$

Where our coefficients map directly to our code:
*   **$a$** = $D \cdot D$ (`dotProduct(r.direction, r.direction)`)
*   **$b$** = $2(D \cdot \vec{V})$ (`2.0f * dotProduct(r.direction, sphere_to_ray)`)
*   **$c$** = $\vec{V} \cdot \vec{V} - r^2$ (`dotProduct(sphere_to_ray, sphere_to_ray) - 1.0f`)

To solve for $t$, we use the quadratic formula:

$$ t = \frac{-b \pm \sqrt{b^2 - 4ac}}{2a} $$

The part under the square root, $b^2 - 4ac$, is the **discriminant**. It tells us how many times the ray intersects the sphere:

1.  **Discriminant < 0**: The ray completely misses the sphere. The square root of a negative number is imaginary, so there are no real solutions (0 intersections).
2.  **Discriminant = 0**: The ray just grazes the edge of the sphere, intersecting at exactly one point (1 intersection).
3.  **Discriminant > 0**: The ray goes directly through the sphere, entering one side and exiting the other (2 intersections).

In our `intersect` function, we calculate these two $t$ values (`t1` and `t2`) and return them. These values represent the distances from the ray's origin to the intersection points.
