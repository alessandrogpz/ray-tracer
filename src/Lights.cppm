export module rt.lights;

import rt.tuple;
import rt.colors;

export namespace rt
{
    struct PointLight
    {
        Color intensity{};
        Point position{};

        PointLight() : intensity(Color()), position(Point()) {}
    };
}