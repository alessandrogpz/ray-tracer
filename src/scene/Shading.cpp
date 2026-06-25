module;

#include <cmath>

module rt.shading;

import rt.materials;
import rt.colors;
import rt.tuple;
import rt.lights;

namespace rt {

    Color lighting(Material material, PointLight light, Point point, Vector eye_vector, Vector normal_vector) {
        Color effective_color = material.color * light.intensity;
        Vector light_vector = normalizeVector(light.position - point);
        Color ambient = effective_color * material.ambient;

        double light_dot_normal = dotProduct(light_vector, normal_vector);

        Color diffuse(0.0, 0.0, 0.0);
        Color specular(0.0, 0.0, 0.0);

        if (light_dot_normal >= 0.0) {
            diffuse = effective_color * material.diffuse * light_dot_normal;

            Vector reflect_vector = reflect(-light_vector, normal_vector);
            double reflect_dot_eye = dotProduct(reflect_vector, eye_vector);

            if (reflect_dot_eye > 0.0) {
                double factor = std::pow(reflect_dot_eye, material.shininess);
                specular = light.intensity * material.specular * factor;
            }
        }

        return ambient + diffuse + specular;
    }

} // namespace rt
