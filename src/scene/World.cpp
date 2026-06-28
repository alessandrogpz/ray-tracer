module;

module rt.world;

import std;

import rt.lights;
import rt.tuple;
import rt.colors;
import rt.utils;
import rt.transformations;
import rt.shading;
import rt.materials;
import rt.utils;

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

        const std::size_t n_objects = w.sphere_origins.size();
        for (std::size_t i = 0; i < n_objects; i++)
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

        // Sort all intersections in ascending order by their t-value
        std::ranges::sort(xs, {}, &Intersection::t);

        return xs;
    }

    Comp prepare_computation(const Intersection& i, const Ray& r, const World& w)
    {
        Comp comps;

        comps.intersection = i;
        comps.point = position(r, i.t);
        comps.eye_v = -r.direction;

        Sphere s;
        s.origin = w.sphere_origins[i.shape_index];
        s.radius = w.sphere_radii[i.shape_index];
        s.material = w.sphere_materials[i.shape_index];
        s.transform = w.sphere_transforms[i.shape_index];
        s.transform_inverse = w.sphere_transforms_inverse[i.shape_index];
        s.transform_inverse_transpose = w.sphere_transforms_inverse_transpose[i.shape_index];

        comps.normal_v = normalAt(s, comps.point);

        if (dotProduct(comps.normal_v, comps.eye_v) < 0)
        {
            comps.inside = true;
            comps.normal_v = -comps.normal_v;
        }

        comps.over_point = comps.point + comps.normal_v * EPSILON;

        return comps;
    }

    Color shade_hit(const World& w, const Comp& c)
    {
        const Material& m = w.sphere_materials[c.intersection.shape_index];
        const bool shadowed = is_shadowed(w, c.over_point);
        return lighting(m, w.light, c.point, c.eye_v, c.normal_v, shadowed);
    }

    Color color_at(const World& w, const Ray&r)
    {
        const std::vector<Intersection> xs = intersect_world(w, r);
        const std::optional<Intersection> h = hit(xs);

        if (!h)
            return {0.0f, 0.0f, 0.0f};

        const Comp comp = prepare_computation(*h, r, w);
        const Color c = shade_hit(w, comp);

        return c;
    }

    bool is_shadowed(const World& w, const Point& p)
    {
        const Vector v = w.light.position - p;

        const float distance = getVectorMagnitude(v);
        const Vector direction = normalizeVector(v);

        const Ray r(p, direction);
        const std::vector<Intersection> intersections = intersect_world(w, r);

        std::optional<Intersection> h = hit(intersections);
        if (h && h->t > EPSILON && h->t < distance)
            return true;
        return false;
    }
    
}