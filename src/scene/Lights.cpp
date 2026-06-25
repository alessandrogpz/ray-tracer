module rt.lights;

import rt.tuple;
import rt.colors;

namespace rt {

    PointLight::PointLight() = default;

    PointLight::PointLight(const Point& _position, const Color& _intensity)
        : intensity(_intensity), position(_position) {}

} // namespace rt
