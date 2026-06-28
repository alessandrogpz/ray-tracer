# How-To: Add a New Shape Primitive

This guide outlines the step-by-step process of adding a new shape primitive (e.g., Cubes, Cylinders, Triangles, Cones) to the ray tracer. Because our project is designed under **Data-Oriented Design (DOD)** principles using a **Structure of Arrays (SoA)** memory layout (rather than inheritance/OOP), adding a shape requires registering its data in parallel vectors in the `World` container and updating the intersection and shading pipelines.

Let `<Shape>` represent the capitalized name of your new primitive (e.g., `Cube`) and `<shape>` represent its lowercase name (e.g., `cube`).

---

## Step 1: Register the Shape Type

Add your new shape to the `ShapeType` enum, which identifies the shape type of an `Intersection` record.

1. Open **[Intersection.cppm](file:///home/aper/Documents/ray-tracer/src/scene/Intersection.cppm)**.
2. In the `ShapeType` enum, add your shape:
   ```cpp
   export enum class ShapeType
   {
       Sphere,
       Plane,
       <Shape> // Add your shape here
   };
   ```

---

## Step 2: Implement the Shape Geometry (`<Shape>.cppm` & `<Shape>.cpp`)

Create the module files for your shape.

### 1. Interface: `src/scene/<Shape>.cppm`
Define the shape struct, its local math interfaces, transform accessors, and intersection/normal free functions.

```cpp
export module rt.<shape>;

import std;
import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.materials;
import rt.intersection;
import rt.ray;

export namespace rt
{
    struct <Shape>
    {
        Material material{};
        Matrix<4> transform{identity()};
        Matrix<4> transform_inverse{identity()};
        Matrix<4> transform_inverse_transpose{identity()};

        // Any shape-specific geometry inputs (e.g., float radius/height, etc.)
        // float height = 2.0f; 

        <Shape>() = default;

        // Transform helpers (copy-paste standard interface)
        Matrix<4> get_transform() const { return transform; }
        Matrix<4> get_transform_inverse() const { return transform_inverse; }
        Matrix<4> get_transform_inverse_transpose() const { return transform_inverse_transpose; }
        
        void set_transform(const Matrix<4>& m) {
            transform = m;
            transform_inverse = m.inverse();
            transform_inverse_transpose = transform_inverse.transpose();
        }

        /**
         * @brief Evaluates ray intersection in object (local) space.
         * Returns t-values or std::nullopt.
         */
        std::optional<float> local_intersect(Point local_origin, Vector local_direction) const;

        /**
         * @brief Evaluates normal vector in object (local) space.
         */
        Vector local_normal_at(Point local_point) const;
    };

    // Free shape functions for intersections and normals (re-usable helpers)
    std::vector<Intersection> intersect(const <Shape>& s, const Ray& r, std::uint32_t index = 0);
    void intersect(const <Shape>& s, const Ray& r, std::vector<Intersection>& xs, std::uint32_t index = 0);
    Vector normalAt(const <Shape>& s, const Point& world_point);
}
```

### 2. Implementation: `src/scene/<Shape>.cpp`
Implement the transformation math, normal transformations, and shape-specific local geometry logic.

```cpp
module rt.<shape>;

import rt.utils;

namespace rt
{
    std::optional<float> <Shape>::local_intersect(Point local_origin, Vector local_direction) const {
        // TODO: Solve intersection against standard unit shape sitting at the origin.
        // Example: check bounds, run mathematical formula, solve quadratic/linear.
        return std::nullopt;
    }

    Vector <Shape>::local_normal_at(Point local_point) const {
        // TODO: Calculate normal at local point
        return Vector(0.0f, 1.0f, 0.0f);
    }

    // --- Free transformation wrappers (standard templates) ---

    std::vector<Intersection> intersect(const <Shape>& s, const Ray& r, std::uint32_t index) {
        std::vector<Intersection> xs;
        intersect(s, r, xs, index);
        return xs;
    }

    void intersect(const <Shape>& s, const Ray& r, std::vector<Intersection>& xs, std::uint32_t index) {
        // Transform ray to local space
        const Ray local_ray = transformRay(r, s.get_transform_inverse());
        
        // Run intersection check
        const auto t_val = s.local_intersect(local_ray.origin, local_ray.direction);
        if (t_val) {
            xs.push_back(Intersection(*t_val, index, ShapeType::<Shape>));
        }
    }

    Vector normalAt(const <Shape>& s, const Point& world_point) {
        const Point local_point = s.get_transform_inverse() * world_point;
        const Vector local_normal = s.local_normal_at(local_point);
        Vector world_normal = s.get_transform_inverse_transpose() * local_normal;
        world_normal.w = 0.0f; // Ensure normal remains a vector
        return normalizeVector(world_normal);
    }
}
```

---

## Step 3: Register in the Build System

Open **[CMakeLists.txt](file:///home/aper/Documents/ray-tracer/CMakeLists.txt)**.
Add your new implementation and interface files under the `raytracer_core` target:
```cmake
add_library(raytracer_core STATIC
        ...
        src/scene/<Shape>.cppm
        src/scene/<Shape>.cpp
)
```

---

## Step 4: Add Shape to the World SoA Vectors

We store shapes in parallel vectors inside `World` to ensure data locality in cache lines.

### 1. Update `World.cppm`
1. Open **[World.cppm](file:///home/aper/Documents/ray-tracer/src/scene/World.cppm)**.
2. Import the new shape module: `import rt.<shape>;`
3. Inside `struct World`, add parallel vectors and declare `add_<shape>`:
   ```cpp
   struct World
   {
       ...
       // <Shape> SoA Vectors
       std::vector<Material> <shape>_materials{};
       std::vector<Matrix<4>> <shape>_transforms{};
       std::vector<Matrix<4>> <shape>_transforms_inverse{};
       std::vector<Matrix<4>> <shape>_transforms_inverse_transpose{};
       // Add any shape-specific geometric arrays here (e.g. heights, widths, origins)

       void add_<shape>(const <Shape>& s);
   };
   ```

### 2. Update `World.cpp`
1. Open **[World.cpp](file:///home/aper/Documents/ray-tracer/src/scene/World.cpp)**.
2. Import the new shape: `import rt.<shape>;`
3. Implement `add_<shape>` to split the struct properties into the SoA vectors:
   ```cpp
   void World::add_<shape>(const <Shape>& s) {
       <shape>_materials.push_back(s.material);
       <shape>_transforms.push_back(s.get_transform());
       <shape>_transforms_inverse.push_back(s.get_transform_inverse());
       <shape>_transforms_inverse_transpose.push_back(s.get_transform_inverse_transpose());
   }
   ```

---

## Step 5: Update the Rendering Pipeline

Now connect your new shape to the intersection, normal-calculation, and shading routines inside **[World.cpp](file:///home/aper/Documents/ray-tracer/src/scene/World.cpp)**.

### 1. Ray-Scene Intersections
In `intersect_world`, add a loop to run ray-intersections against your new shape collection:
```cpp
std::vector<Intersection> intersect_world(const World& w, const Ray& r)
{
    std::vector<Intersection> xs;

    // ... Intersect spheres & planes ...

    // Intersect <Shape> SoA
    const std::size_t n_<shape>s = w.<shape>_materials.size();
    for (std::size_t i = 0; i < n_<shape>s; i++)
    {
        <Shape> s;
        s.material = w.<shape>_materials[i];
        s.transform = w.<shape>_transforms[i];
        s.transform_inverse = w.<shape>_transforms_inverse[i];
        s.transform_inverse_transpose = w.<shape>_transforms_inverse_transpose[i];

        intersect(s, r, xs, static_cast<std::uint32_t>(i));
    }

    std::ranges::sort(xs, {}, &Intersection::t);
    return xs;
}
```

### 2. Normal Calculations
In `prepare_computation`, add a conditional branch to query your shape-specific normal vector when a hit occurs:
```cpp
Comp prepare_computation(const Intersection& i, const Ray& r, const World& w)
{
    Comp comps;
    ...
    if (i.shape_type == ShapeType::Sphere) {
        // ... Sphere reconstruction and normalAt ...
    }
    else if (i.shape_type == ShapeType::Plane) {
        // ... Plane reconstruction and normalAt ...
    }
    else if (i.shape_type == ShapeType::<Shape>) {
        <Shape> s;
        s.material = w.<shape>_materials[i.shape_index];
        s.transform = w.<shape>_transforms[i.shape_index];
        s.transform_inverse = w.<shape>_transforms_inverse[i.shape_index];
        s.transform_inverse_transpose = w.<shape>_transforms_inverse_transpose[i.shape_index];

        comps.normal_v = normalAt(s, comps.point);
    }
    ...
}
```

### 3. Shading Material Lookups
In `shade_hit`, add your shape to the material lookup resolution:
```cpp
Color shade_hit(const World& w, const Comp& c)
{
    const Material& m = (c.intersection.shape_type == ShapeType::Sphere)
        ? w.sphere_materials[c.intersection.shape_index]
        : (c.intersection.shape_type == ShapeType::Plane)
            ? w.plane_materials[c.intersection.shape_index]
            : w.<shape>_materials[c.intersection.shape_index]; // Fetches your shape material

    const bool shadowed = is_shadowed(w, c.over_point);
    return lighting(m, w.light, c.point, c.eye_v, c.normal_v, shadowed);
}
```

---

## Step 6: Write Unit Tests

Verify the new shape's normals, intersections, and transformation logic.

1. Create a new test suite file at `tests/<Shape>.t.cpp` following the structure of `tests/Plane.t.cpp` or `tests/Sphere.t.cpp`.
2. Register the test file in **[CMakeLists.txt](file:///home/aper/Documents/ray-tracer/CMakeLists.txt)** under the `run_tests` target sources list.
3. Build and execute your test suite:
   ```bash
   cmake --build build-debug
   ./build-debug/run_tests
   ```
