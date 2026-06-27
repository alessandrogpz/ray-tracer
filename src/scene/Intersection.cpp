module rt.intersection;

import std;

namespace rt {

    Intersection::Intersection(float _t, std::uint32_t _shape_index, ShapeType _shape_type)
        : t(_t), shape_index(_shape_index), shape_type(_shape_type) {}

} // namespace rt
