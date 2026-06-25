export module rt.intersection;

import rt.shapes;

export namespace rt {

    struct Intersection {
        double t;
        const Sphere* obj;

        Intersection(double _t, const Sphere* _obj);
    };

} // namespace rt
