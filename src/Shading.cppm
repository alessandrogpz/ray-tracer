module;

#include <cmath>

export module rt.shading;

import rt.materials;
import rt.colors;
import rt.tuple;
import rt.lights;

export namespace rt
{
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
    [[nodiscard]]
    Color lighting(Material material, PointLight light, Point point, Vector eye_vector, Vector normal_vector)
    {
        // Combine the surface color with the light's color / intensity
        Color effective_color = material.color * light.intensity;

        // Find the direction to the light source
        Vector light_vector = normalizeVector(light.position - point);

        // Compute the ambient contribution
        Color ambient = effective_color * material.ambient;

        // light_dot_normal represents the cosine of the angle between the light vector and the normal vector.
        double light_dot_normal = dotProduct(light_vector, normal_vector);

        Color diffuse(0.0, 0.0, 0.0);
        Color specular(0.0, 0.0, 0.0);

        if (light_dot_normal >= 0.0)
        {
            // Compute the diffuse contribution
            diffuse = effective_color * material.diffuse * light_dot_normal;

            // reflect_dot_eye represents the cosine of the angle between the reflection vector and the eye vector.
            Vector reflect_vector = reflect(-light_vector, normal_vector);
            double reflect_dot_eye = dotProduct(reflect_vector, eye_vector);

            if (reflect_dot_eye > 0.0)
            {
                // Compute the specular contribution
                double factor = std::pow(reflect_dot_eye, material.shininess);
                specular = light.intensity * material.specular * factor;
            }
        }

        // Add the three contributions together to get the final shading
        return ambient + diffuse + specular;
    }
}

