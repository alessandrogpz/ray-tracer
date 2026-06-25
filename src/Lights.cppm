export module rt.lights;

import rt.tuple;
import rt.colors;

export namespace rt
{
    struct PointLight
    {
        Color intensity{};
        Point position{};

        PointLight() = default;
        PointLight(const Point& _position, const Color& _intensity)
            : intensity(_intensity), position(_position) {}
    };
}