export module rt.intersection;

import rt.shape_base;

export namespace rt {

    struct Intersection {
        double t;
        const Shape* obj;

        Intersection(double _t, const Shape* _obj);
    };

} // namespace rt
