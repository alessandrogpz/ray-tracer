module;

module rt.world;

import std;

import rt.lights;
import rt.tuple;
import rt.colors;
import rt.shape_base;
import rt.utils;
import rt.transformations;

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
}