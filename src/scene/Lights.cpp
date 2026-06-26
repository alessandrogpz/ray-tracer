module rt.lights;

import rt.tuple;
import rt.colors;

namespace rt {

    PointLight::PointLight() = default;

    PointLight::PointLight(const Point& _position, const Color& _intensity)
        : position(_position), intensity(_intensity) {}

    bool PointLight::operator==(const PointLight& other) const {
        return position == other.position && intensity == other.intensity;
    }

} // namespace rt
