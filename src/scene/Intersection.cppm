export module rt.intersection;

import std;

export namespace rt {

    enum class ShapeType : std::uint8_t {
        Sphere
    };

    struct Intersection {
        double t;
        std::uint32_t shape_index;
        ShapeType shape_type;

        Intersection(double _t, std::uint32_t _shape_index, ShapeType _shape_type);
    };

} // namespace rt
