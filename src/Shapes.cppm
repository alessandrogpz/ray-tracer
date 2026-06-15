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

    /**
     * @brief Computes the surface normal vector of a sphere at a given world-space point.
     *
     * Transforms the world-space point into the sphere's local object space, computes the normal vector
     * relative to the sphere's origin, then transforms it back into world space using the transpose of the
     * inverse transformation matrix. The final vector is normalized before being returned.
     *
     * @param s The sphere on which the normal is calculated.
     * @param p The point in world space coordinates.
     * @return vector The normalized world-space normal vector at the given point.
    */
    [[nodiscard]]
    vector normalAt(const sphere& s, point p)
    {
        // 1. Convert point from world space to object space
        const point object_point = s.transform.inverse() * p;
        
        // 2. Calculate object space normal (vector from center to object point)
        const vector object_normal = object_point - createPoint(0.0, 0.0, 0.0);
        
        // 3. Transform object-space normal to world-space using transpose of the inverse
        const tuple world_normal_tuple = s.transform.inverse().transpose() * static_cast<const tuple&>(object_normal);
        
        // 4. Reset w = 0.0 and construct a clean vector
        const vector world_normal = createVector(world_normal_tuple.x, world_normal_tuple.y, world_normal_tuple.z);

        // 5. Normalize and return
        return normalizeVector(world_normal);
    }


}