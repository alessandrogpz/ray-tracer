module;

export module rt.intersection;

import rt.shapes;

export namespace rt
{
    // Tracks the intersection distance (t) and references the original hit object.
    struct intersection
    {
        double t;
        const sphere* obj;

        intersection(double _t, const sphere* _obj) : t(_t), obj(_obj) {}
    };
}
