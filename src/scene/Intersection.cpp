module rt.intersection;

import rt.shape_base;

namespace rt {

    Intersection::Intersection(double _t, const Shape* _obj) : t(_t), obj(_obj) {}

} // namespace rt
