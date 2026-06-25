# Explanation: Ray-Sphere Intersection Algorithm

The `intersect(Sphere, Ray)` function determines the points where a Ray intersects a Sphere. The approach used is an **analytic solution**, relying on algebraic equations rather than numerical approximation.

## The Math Behind the Intersection

To find where a Ray and a Sphere intersect, we need to find the points in 3D space that satisfy both the equation of the Ray and the equation of the Sphere.

### 1. World Space vs. Object Space (Transformations)

In a 3D engine, spheres are frequently scaled, rotated, or translated using a transformation Matrix. Trying to calculate intersections against a distorted Sphere requires incredibly complex math.

Instead of changing the Sphere, we change the Ray. We multiply the Ray by the **inverse** of the Sphere's transformation Matrix. This converts the Ray from "World Space" into the Sphere's "Local Object Space." In this local space, every Sphere acts like a standard, un-transformed Sphere sitting at the origin, which makes the subsequent math much easier.

### 2. The Equation of a Ray

A Ray is defined by an origin Point ($O$) and a direction Vector ($D$). Any Point ($P$) on the Ray can be found by traveling a certain distance ($t$) along its direction:

$$ P = O + tD $$

*(Note: In our math below, $O$ and $D$ refer to the origin and direction of the transformed `local_ray`.)*

### 3. The Equation of a Sphere

A Sphere is defined by its center Point ($C$) and its radius ($r$). Any Point ($P$) lies on the surface of the Sphere if the distance from the center to that Point is exactly equal to the radius. Using the dot product, this is expressed as:

$$ (P - C) \cdot (P - C) = r^2 $$

### 4. Combining the Equations

To find the Intersection, we substitute the Ray equation into the Sphere equation. This means we are looking for a distance ($t$) where the resulting Point $P$ is on the Sphere.

Substitute $P = O + tD$ into $(P - C) \cdot (P - C) = r^2$:

$$ (O + tD - C) \cdot (O + tD - C) = r^2 $$

Let the Vector from the Sphere's center to the Ray's origin be $\vec{V} = O - C$. In our code, this is `sphere_to_ray`.
The equation simplifies to:

$$ (tD + \vec{V}) \cdot (tD + \vec{V}) = r^2 $$

Expanding the dot product gives a quadratic equation in terms of $t$:

$$ (D \cdot D)t^2 + 2(D \cdot \vec{V})t + (\vec{V} \cdot \vec{V} - r^2) = 0 $$

### 5. Solving the Quadratic Equation

This matches the standard quadratic form:

$$ at^2 + bt + c = 0 $$

Where our coefficients map directly to our code using the `local_ray`:
*   **$a$** = $D \cdot D$ (`dotProduct(local_ray.direction, local_ray.direction)`)
*   **$b$** = $2(D \cdot \vec{V})$ (`2.0f * dotProduct(local_ray.direction, sphere_to_ray)`)
*   **$c$** = $\vec{V} \cdot \vec{V} - r^2$ (`dotProduct(sphere_to_ray, sphere_to_ray) - (s.radius * s.radius)`)

To solve for $t$, we use the quadratic formula:

$$ t = \frac{-b \pm \sqrt{b^2 - 4ac}}{2a} $$

The part under the square root, $b^2 - 4ac$, is the **discriminant**. It tells us how many times the Ray intersects the Sphere:

1.  **Discriminant < 0**: The Ray completely misses the Sphere. The square root of a negative number is imaginary, so there are no real solutions (0 intersections).
2.  **Discriminant = 0**: The Ray just grazes the edge of the Sphere, intersecting at exactly one Point (1 Intersection).
3.  **Discriminant > 0**: The Ray goes directly through the Sphere, entering one side and exiting the other (2 intersections).

In our `intersect` function, we calculate these two $t$ values (`t1` and `t2`) and return them. These values represent the distances from the Ray's origin to the Intersection points.