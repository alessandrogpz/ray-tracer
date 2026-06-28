module;
#include <cmath>
module rt.shading;

import std;

import rt.materials;
import rt.colors;
import rt.tuple;
import rt.lights;

namespace rt {

    Color lighting(const Material& material, const PointLight& light, const Point& point, const Vector& eye_vector, const Vector& normal_vector, const bool inShadow) {
        Color effective_color = material.color * light.intensity;
        Color ambient = effective_color * material.ambient;

        if (inShadow)
            return ambient;

        Vector light_vector = normalizeVector(light.position - point);

        float light_dot_normal = dotProduct(light_vector, normal_vector);

        Color diffuse(0.0f, 0.0f, 0.0f);
        Color specular(0.0f, 0.0f, 0.0f);

        if (light_dot_normal >= 0.0f) {
            diffuse = effective_color * material.diffuse * light_dot_normal;

            Vector reflect_vector = reflect(-light_vector, normal_vector);
            float reflect_dot_eye = dotProduct(reflect_vector, eye_vector);

            if (reflect_dot_eye > 0.0f) {
                float factor = std::pow(reflect_dot_eye, material.shininess);
                specular = light.intensity * material.specular * factor;
            }
        }

        return ambient + diffuse + specular;
    }

} // namespace rt
