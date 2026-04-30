module;

export module rt.shapes;

import rt.tuple;

export namespace rt
{
    struct sphere
    {
        int id;
        point origin;
        float radius;

        // Main constructor for ID generation
        explicit sphere(point _origin, float _radius) : origin(_origin), radius(_radius)
        {
            static int next_id = 0;
            id = ++next_id;
        }

        // Default constructor delegates to the main constructor
        sphere() : sphere(createPoint(0.0f, 0.0f, 0.0f), 1.0f) {}

        bool operator==(const sphere& other) const
        {
            return id == other.id;
        }
    };
}