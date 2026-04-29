module;

export module rt.ray;

import rt.tuple;

export namespace rt
{
    struct ray
    {
        point origin;
        vector direction;

        ray() : origin(createPoint(0.0f, 0.0f, 0.0f)),
                direction(createVector(0.0f, 0.0f, 0.0f)) {}

        ray(point _origin, vector _direction)
            : origin(_origin), direction(_direction) {}
    };
}
