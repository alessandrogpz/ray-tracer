module rt.world;

import std;

import rt.lights;
import rt.tuple;
import rt.colors;
import rt.utils;
import rt.transformations;
import rt.shading;
import rt.materials;
import rt.plane;

namespace rt
{
    void World::add_sphere(const Sphere& s) {
        sphere_origins.push_back(s.origin);
        sphere_radii.push_back(s.radius);
        sphere_materials.push_back(s.material);
        sphere_transforms.push_back(s.get_transform());
        sphere_transforms_inverse.push_back(s.get_transform_inverse());
        sphere_transforms_inverse_transpose.push_back(s.get_transform_inverse_transpose());
    }

    void World::add_plane(const Plane& p) {
        plane_materials.push_back(p.material);
        plane_transforms.push_back(p.get_transform());
        plane_transforms_inverse.push_back(p.get_transform_inverse());
        plane_transforms_inverse_transpose.push_back(p.get_transform_inverse_transpose());
    }

    World default_world() {
        World w;
        w.light = PointLight(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

        Sphere s1;
        s1.material.color = Color(0.8f, 1.0f, 0.6f);
        s1.material.diffuse = 0.7f;
        s1.material.specular = 0.2f;
        w.add_sphere(s1);

        Sphere s2;
        s2.set_transform(scale(0.5f, 0.5f, 0.5f));
        w.add_sphere(s2);

        return w;
    }

    std::vector<Intersection> intersect_world(const World& w, const Ray& r)
    {
        std::vector<Intersection> xs;

        // 1. Intersect Spheres SoA
        const std::size_t n_spheres = w.sphere_origins.size();
        for (std::size_t i = 0; i < n_spheres; i++)
        {
            Sphere s;
            s.origin = w.sphere_origins[i];
            s.radius = w.sphere_radii[i];
            s.material = w.sphere_materials[i];
            s.transform = w.sphere_transforms[i];
            s.transform_inverse = w.sphere_transforms_inverse[i];
            s.transform_inverse_transpose = w.sphere_transforms_inverse_transpose[i];

            intersect(s, r, xs, static_cast<std::uint32_t>(i));
        }

        // 2. Intersect Planes SoA
        const std::size_t n_planes = w.plane_materials.size();
        for (std::size_t i = 0; i < n_planes; i++)
        {
            Plane p;
            p.material = w.plane_materials[i];
            p.transform = w.plane_transforms[i];
            p.transform_inverse = w.plane_transforms_inverse[i];
            p.transform_inverse_transpose = w.plane_transforms_inverse_transpose[i];

            intersect(p, r, xs, static_cast<std::uint32_t>(i));
        }

        // Sort all intersections in ascending order by their t-value
        std::ranges::sort(xs, {}, &Intersection::t);

        return xs;
    }

    const Material& get_material(const World& w, ShapeType type, std::size_t index);

    Comp prepare_computation(const Intersection& i, const Ray& r, const World& w)
    {
        return prepare_computation(i, r, w, std::vector<Intersection>{i});
    }

    Comp prepare_computation(const Intersection& i, const Ray& r, const World& w, const std::vector<Intersection>& xs)
    {
        Comp comps;

        comps.intersection = i;
        comps.point = position(r, i.t);
        comps.eye_v = -r.direction;

        // Query shape-specific normal vector depending on the shape type
        switch (i.shape_type)
        {
            case ShapeType::Sphere:
            {
                Sphere s;
                s.origin = w.sphere_origins[i.shape_index];
                s.radius = w.sphere_radii[i.shape_index];
                s.material = w.sphere_materials[i.shape_index];
                s.transform = w.sphere_transforms[i.shape_index];
                s.transform_inverse = w.sphere_transforms_inverse[i.shape_index];
                s.transform_inverse_transpose = w.sphere_transforms_inverse_transpose[i.shape_index];

                comps.normal_v = normalAt(s, comps.point);
                break;
            }
            case ShapeType::Plane:
            {
                Plane p;
                p.material = w.plane_materials[i.shape_index];
                p.transform = w.plane_transforms[i.shape_index];
                p.transform_inverse = w.plane_transforms_inverse[i.shape_index];
                p.transform_inverse_transpose = w.plane_transforms_inverse_transpose[i.shape_index];

                comps.normal_v = normalAt(p, comps.point);
                break;
            }
        }

        if (dotProduct(comps.normal_v, comps.eye_v) < 0)
        {
            comps.inside = true;
            comps.normal_v = -comps.normal_v;
        }

        comps.reflect_v = reflect(r.direction, comps.normal_v);
        // Nudge the point off the surface: EPSILON near the origin (exactly as in the book), growing
        // with distance beyond 100 units so the nudge never becomes smaller than float spacing there.
        // Reference: documentation/references/CoreMath.md (Comparing Floating-Point Numbers)
        const float magnitude = std::max({std::abs(comps.point.x), std::abs(comps.point.y), std::abs(comps.point.z)});
        const float offset = std::max(EPSILON, SURFACE_OFFSET_SCALE * magnitude);
        comps.over_point = comps.point + comps.normal_v * offset;
        comps.under_point = comps.point - comps.normal_v * offset;

        // Find the refractive indices on either side of the hit by tracking which
        // objects the ray has entered but not yet exited ("containers").
        // Math reference: documentation/explanation/ReflectionAndRefraction.md
        std::vector<Intersection> containers;
        const auto same_object = [](const Intersection& a, const Intersection& b) {
            return a.shape_type == b.shape_type && a.shape_index == b.shape_index;
        };
        const auto refractive_index_of_last = [&]() {
            return containers.empty()
                ? 1.0f
                : get_material(w, containers.back().shape_type, containers.back().shape_index).refractive_index;
        };

        for (const Intersection& x : xs)
        {
            const bool is_hit = same_object(x, i) && x.t == i.t;

            if (is_hit)
                comps.n1 = refractive_index_of_last();

            // Entering an object adds it to the containers; exiting removes it
            const auto it = std::ranges::find_if(containers, [&](const Intersection& c) { return same_object(c, x); });
            if (it != containers.end())
                containers.erase(it);
            else
                containers.push_back(x);

            if (is_hit)
            {
                comps.n2 = refractive_index_of_last();
                break;
            }
        }

        return comps;
    }

    const Material& get_material(const World& w, ShapeType type, std::size_t index)
    {
        switch (type)
        {
            case ShapeType::Sphere:
                return w.sphere_materials[index];
            case ShapeType::Plane:
                return w.plane_materials[index];
            default:
                throw std::runtime_error("Unknown shape type in get_material");
        }
    }

    const Matrix<4>& get_transform_inverse(const World& w, ShapeType type, std::size_t index)
    {
        switch (type)
        {
            case ShapeType::Sphere:
                return w.sphere_transforms_inverse[index];
            case ShapeType::Plane:
                return w.plane_transforms_inverse[index];
            default:
                throw std::runtime_error("Unknown shape type in get_transform_inverse");
        }
    }

    Color shade_hit(const World& w, const Comp& c, const int remaining)
    {
        const ShapeType type = c.intersection.shape_type;
        const std::size_t index = c.intersection.shape_index;

        const Material& m = get_material(w, type, index);
        const bool shadowed = is_shadowed(w, c.over_point);

        // Shade at over_point (nudged off the surface) so patterns like checkers on a
        // y = 0 floor are sampled consistently instead of flickering from rounding
        const Color surface = lighting(m, get_transform_inverse(w, type, index), w.light, c.over_point, c.eye_v, c.normal_v, shadowed);

        const Color reflected = reflected_color(w, c, remaining);
        const Color refracted = refracted_color(w, c, remaining);

        // Fresnel effect: glass-like surfaces reflect more at grazing angles and refract more head-on
        if (m.reflective > 0.0f && m.transparency > 0.0f)
        {
            const float reflectance = schlick(c);
            return surface + reflected * reflectance + refracted * (1.0f - reflectance);
        }

        return surface + reflected + refracted;
    }

    Color color_at(const World& w, const Ray& r, const int remaining)
    {
        const std::vector<Intersection> xs = intersect_world(w, r);
        const std::optional<Intersection> h = hit(xs);

        if (!h)
            return {0.0f, 0.0f, 0.0f};

        const Comp comp = prepare_computation(*h, r, w, xs);
        const Color c = shade_hit(w, comp, remaining);

        return c;
    }

    Color reflected_color(const World& w, const Comp& c, const int remaining)
    {
        const Material& m = get_material(w, c.intersection.shape_type, c.intersection.shape_index);

        if (remaining <= 0 || m.reflective == 0.0f)
            return {0.0f, 0.0f, 0.0f};

        const Ray reflect_ray(c.over_point, c.reflect_v);
        return color_at(w, reflect_ray, remaining - 1) * m.reflective;
    }

    Color refracted_color(const World& w, const Comp& c, const int remaining)
    {
        const Material& m = get_material(w, c.intersection.shape_type, c.intersection.shape_index);

        if (remaining <= 0 || m.transparency == 0.0f)
            return {0.0f, 0.0f, 0.0f};

        // Snell's law: find sin²(θt) from the ratio of refractive indices and cos(θi)
        const float n_ratio = c.n1 / c.n2;
        const float cos_i = dotProduct(c.eye_v, c.normal_v);
        const float sin2_t = n_ratio * n_ratio * (1.0f - cos_i * cos_i);

        // Total internal reflection: no light crosses the boundary
        if (sin2_t > 1.0f)
            return {0.0f, 0.0f, 0.0f};

        const float cos_t = std::sqrt(1.0f - sin2_t);
        const Vector direction = c.normal_v * (n_ratio * cos_i - cos_t) - c.eye_v * n_ratio;
        const Ray refract_ray(c.under_point, direction);

        return color_at(w, refract_ray, remaining - 1) * m.transparency;
    }

    float schlick(const Comp& c)
    {
        float cos = dotProduct(c.eye_v, c.normal_v);

        // Total internal reflection can only occur when leaving a denser medium
        if (c.n1 > c.n2)
        {
            const float n = c.n1 / c.n2;
            const float sin2_t = n * n * (1.0f - cos * cos);
            if (sin2_t > 1.0f)
                return 1.0f;

            // When n1 > n2, use cos(θt) instead of cos(θi)
            cos = std::sqrt(1.0f - sin2_t);
        }

        const float r0 = std::pow((c.n1 - c.n2) / (c.n1 + c.n2), 2.0f);
        return r0 + (1.0f - r0) * std::pow(1.0f - cos, 5.0f);
    }

    bool is_shadowed(const World& w, const Point& p)
    {
        const Vector v = w.light.position - p;

        const float distance = getVectorMagnitude(v);
        const Vector direction = normalizeVector(v);

        const Ray r(p, direction);
        const std::vector<Intersection> intersections = intersect_world(w, r);

        // Any shadow-casting object between the point and the light blocks it; objects
        // that opt out (casts_shadow = false, e.g. water) let the light through
        for (const Intersection& i : intersections)
        {
            if (i.t <= EPSILON)
                continue;
            if (i.t >= distance)
                break;
            if (get_material(w, i.shape_type, i.shape_index).casts_shadow)
                return true;
        }
        return false;
    }
    
} // namespace rt