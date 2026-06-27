export module rt.shading;

import rt.materials;
import rt.colors;
import rt.tuple;
import rt.lights;

export namespace rt {

    /**
     * @brief Computes the surface Color at a given Point using the Phong reflection model.
     *
     * Combines three lighting components: ambient (constant background light), diffuse
     * (matte reflection), and specular (glossy highlight) based on the angle of the light
     * source, the surface normal, and the camera eye position.
     * Conceptual overview: documentation/explanation/Architecture.md
     *
     * @param material The Material properties of the illuminated surface.
     * @param light The PointLight source illuminating the scene.
     * @param point The 3D Point on the surface being shaded.
     * @param eye_vector The eye Vector pointing from the surface point back to the camera.
     * @param normal_vector The surface normal Vector at the shaded point.
     * @return Color The final calculated Color contribution of the Phong reflection model at the given point.
    */
    [[nodiscard]] Color lighting(const Material& material, const PointLight& light, const Point& point, const Vector& eye_vector, const Vector& normal_vector);

} // namespace rt
