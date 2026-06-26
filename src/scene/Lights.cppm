export module rt.lights;

import rt.tuple;
import rt.colors;

export namespace rt {

    struct PointLight {
        Point position;
        Color intensity;

        PointLight();
        PointLight(const Point& _position, const Color& _intensity);

        [[nodiscard]] bool operator==(const PointLight& other) const;
    };

} // namespace rt
