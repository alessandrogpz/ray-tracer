module;

export module rt.intersection;

import rt.shapes;

export namespace rt
{
    // Tracks the Intersection distance (t) and references the original hit object.
    struct Intersection
    {
        double t;
        const Sphere* obj;

        Intersection(double _t, const Sphere* _obj) : t(_t), obj(_obj) {}
    };
}
