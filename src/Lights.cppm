export module rt.lights;

import rt.tuple;
import rt.colors;

export namespace rt
{
    struct point_light
    {
        color intensity{};
        point position{};

        point_light() : intensity(color()), position(point()) {}
    };
}