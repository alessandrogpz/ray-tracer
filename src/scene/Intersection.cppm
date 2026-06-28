export module rt.intersection;

import std;

export namespace rt {

    enum class ShapeType : std::uint8_t {
        Sphere,
        Plane
    };

    struct Intersection {
        float t{0.0f};
        std::uint32_t shape_index{0};
        ShapeType shape_type{ShapeType::Sphere};

        Intersection() = default;
        Intersection(float _t, std::uint32_t _shape_index, ShapeType _shape_type);
    };

} // namespace rt
