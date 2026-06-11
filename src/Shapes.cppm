module;

export module rt.shapes;

import rt.tuple;
import rt.matrix;
import rt.transformations;

export namespace rt
{
    struct sphere
    {
        int id;
        point origin;
        double radius;
        matrix<4> transform {identity()};


        // Main constructor for ID generation
        explicit sphere(point _origin, double _radius) : origin(_origin), radius(_radius)
        {
            static int next_id = 0;
            id = ++next_id;
        }

        // Default constructor delegates to the main constructor
        sphere() : sphere(createPoint(0.0, 0.0, 0.0), 1.0) {}

        bool operator==(const sphere& other) const
        {
            return id == other.id;
        }

        void set_transform(const matrix<4>& t)
        {
            transform = t;
        }
    };
}