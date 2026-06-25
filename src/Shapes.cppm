module;

export module rt.shapes;

import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.materials;

export namespace rt
{
    struct Sphere
    {
        int id;
        Point origin;
        double radius;
        Matrix<4> transform {identity()};
        Material material{};


        // Main constructor for ID generation
        explicit Sphere(Point _origin, double _radius, Material _material = Material())
            : origin(_origin), radius(_radius), material(_material)
        {
            static int next_id = 0;
            id = ++next_id;
        }

        // Default constructor delegates to the main constructor
        Sphere() : Sphere(Point(), 1.0, Material()) {}

        bool operator==(const Sphere& other) const
        {
            return id == other.id;
        }

        void set_transform(const Matrix<4>& t)
        {
            transform = t;
        }
    };

    /**
     * @brief Computes the surface normal Vector of a Sphere at a given world-space Point.
     *
     * Transforms the world-space Point into the Sphere's local object space, computes the normal Vector
     * relative to the Sphere's origin, then transforms it back into world space using the transpose of the
     * inverse transformation Matrix. The final Vector is normalized before being returned.
     *
     * @param s The Sphere on which the normal is calculated.
     * @param p The Point in world space coordinates.
     * @return Vector The normalized world-space normal Vector at the given Point.
    */
    [[nodiscard]]
    Vector normalAt(const Sphere& s, Point p)
    {
        // 1. Convert Point from world space to object space
        const Point object_point = s.transform.inverse() * p;
        
        // 2. Calculate object space normal (Vector from center to object Point)
        const Vector object_normal = object_point - createPoint(0.0, 0.0, 0.0);
        
        // 3. Transform object-space normal to world-space using transpose of the inverse
        const Tuple world_normal_tuple = s.transform.inverse().transpose() * static_cast<const Tuple&>(object_normal);
        
        // 4. Reset w = 0.0 and construct a clean Vector
        const Vector world_normal = createVector(world_normal_tuple.x, world_normal_tuple.y, world_normal_tuple.z);

        // 5. Normalize and return
        return normalizeVector(world_normal);
    }


}