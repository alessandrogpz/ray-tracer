module rt.intersection;

import rt.shapes;

namespace rt {

    Intersection::Intersection(double _t, const Sphere* _obj) : t(_t), obj(_obj) {}

} // namespace rt
