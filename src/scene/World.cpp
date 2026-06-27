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

namespace rt
{
    World default_world() {
        World w;
        w.light = PointLight(Point(-10, 10, -10), Color(1.0, 1.0, 1.0));

        Sphere s1;
        s1.material.color = Color(0.8, 1.0, 0.6);
        s1.material.diffuse = 0.7;
        s1.material.specular = 0.2;
        w.spheres.push_back(s1);

        Sphere s2;
        s2.set_transform(scale(0.5, 0.5, 0.5));
        w.spheres.push_back(s2);

        return w;
    }

    std::vector<Intersection> intersect_world(const World& w, const Ray& r)
    {
        std::vector<Intersection> xs;

        const std::size_t n_objects = w.spheres.size();
        for (std::size_t i = 0; i < n_objects; i++)
            intersect(w.spheres[i], r, xs, static_cast<std::uint32_t>(i));

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
        comps.normal_v = normalAt(w.spheres[i.shape_index], comps.point);

        if (dotProduct(comps.normal_v, comps.eye_v) < 0)
        {
            comps.inside = true;
            comps.normal_v = -comps.normal_v;
        }

        return comps;
    }

    Color shade_hit(const World& w, const Comp& c)
    {
        const Material& m = w.spheres[c.intersection.shape_index].material;
        return lighting(m, w.light, c.point, c.eye_v, c.normal_v);
    }


}