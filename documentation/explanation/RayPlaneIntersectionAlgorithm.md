# Explanation: Ray-Plane Intersection Algorithm

Under our Data-Oriented Design (DOD) shape architecture, the `intersect(const Plane&, const Ray&, ...)` functions determine the point where a Ray intersects an infinite flat Plane. This approach uses an **analytic solution**, relying on linear algebra rather than iterative numerical approximations.

Like spheres, there are two overloads optimized for different usage contexts:
1. `std::vector<Intersection> intersect(const Plane& p, const Ray& r, std::uint32_t index = 0)`: Allocates and returns a vector containing the intersection (useful for standalone queries and tests).
2. `void intersect(const Plane& p, const Ray& r, std::vector<Intersection>& xs, std::uint32_t index = 0)`: Appends the intersection directly to an existing vector, eliminating heap allocations in hot rendering loops.

---

## The Math Behind the Intersection

To find where a Ray and a Plane intersect, we solve for the point in 3D space that satisfies both the equation of the Ray and the equation of the Plane.

### 1. World Space vs. Object Space (Transformations)

In a 3D scene, planes can be translated, scaled, or rotated using a transformation Matrix. Trying to calculate intersections against a arbitrarily rotated and shifted plane in world space requires complex trigonometry.

Instead of changing the Plane, we change the Ray. We multiply the Ray by the **inverse** of the Plane's transformation Matrix. This converts the Ray from "World Space" into the Plane's "Local Object Space." In local space, the Plane is always defined as the infinite flat horizontal plane lying exactly on the $y = 0$ surface.

### 2. The Equation of a Ray

A Ray is defined by an origin Point ($O$) and a direction Vector ($D$). Any Point ($P$) on the Ray at distance ($t$) is given by:

$$P = O + tD$$

*(Note: In our math below, $O$ and $D$ refer to the origin and direction of the transformed `local_ray`.)*

### 3. The Equation of a Plane

In local space, the plane is the $xz$-plane where the height ($y$) is always zero. The local normal vector is constant and perpendicular to the surface:

$$\vec{N}_{\text{local}} = (0, 1, 0)$$

Any point $P = (P_x, P_y, P_z)$ lies on the local plane if and only if its $y$-coordinate is zero:

$$P_y = 0$$

### 4. Combining the Equations

To find the intersection, we substitute the Ray equation into the Plane equation. This means we are looking for a distance ($t$) where the $y$-coordinate of the point on the ray becomes zero:

$$O_y + tD_y = 0$$

Solving for $t$:

$$t = -\frac{O_y}{D_y}$$

---

## Special Cases and Boundary Conditions

When computing the intersection, we must handle two scenarios:

### 1. Parallel Rays ($D_y = 0$)
If the ray's direction vector has no vertical component ($D_y = 0$), the ray is parallel to the plane. 
*   **Coplanar Ray ($O_y = 0$):** If the ray also lies exactly on the $y = 0$ plane, it is coplanar. In ray tracing, a coplanar ray does not block other rays (it has zero cross-sectional area), so we treat it as no intersection.
*   **Parallel Non-intersecting Ray ($O_y \neq 0$):** The ray is floating above or below the plane and will never cross it.

In both cases, we filter out these scenarios by checking if the absolute value of $D_y$ is less than `EPSILON` ($0.0001f$):

$$\text{if } |D_y| < \text{EPSILON} \implies \text{No Intersections}$$

### 2. Single Intersection ($|D_y| \ge \text{EPSILON}$)
If the ray is not parallel, it is guaranteed to cross the infinite plane exactly once at the distance:

$$t = -\frac{O_y}{D_y}$$

In our `intersect` functions, we calculate this $t$ value and record it inside an `Intersection` structure (with `ShapeType::Plane`) if it exists. Note that $t$ can be negative if the intersection point lies behind the ray's origin.
