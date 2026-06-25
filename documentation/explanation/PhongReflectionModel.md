# Explanation: Phong Reflection Model Algorithm

The `lighting(Material, PointLight, Point, Vector, Vector)` function computes the color of a surface point using the **Phong reflection model**. Developed by Bui Tuong Phong in 1973, this is an empirical model of local illumination that simulates how light interacts with a surface.

---

## 1. The Core Vectors

To calculate how light reflects off a surface point, the algorithm relies on four normalized (unit) vectors in 3D space:

*   **Light Vector ($\vec{L}$)**: Points from the surface point being shaded to the position of the light source.
*   **Normal Vector ($\vec{N}$)**: Perpendicular to the surface at the point of shading.
*   **Eye Vector ($\vec{V}$)**: Points from the surface point back to the camera/observer.
*   **Reflection Vector ($\vec{R}$)**: The direction of perfect mirror reflection of the light vector ($\vec{L}$) off the surface. It is calculated by reflecting the incoming light vector ($-\vec{L}$) about the normal vector ($\vec{N}$).

```
          Light (L)           Normal (N)          Eye (V)
              \                   |                 /
               \                  |                /
                \                 |  (reflect)    /
                 \                |   . - - R    /
                  \               | .           /
                   \              |            /
  ------------------*-------------*-----------*------------------
                    Surface Point
```

---

## 2. The Three Components of Phong Reflection

The Phong model calculates the final color of a point by summing three distinct lighting components: **Ambient**, **Diffuse**, and **Specular**.

$$ \text{Color} = \text{Ambient} + \text{Diffuse} + \text{Specular} $$

### 2.1. Ambient Reflection
Ambient lighting represents constant background light that has bounced off other objects so many times that it appears to come from all directions equally. It ensures that parts of objects in shadow are not completely black.

The ambient contribution is independent of the light's position or the camera's position:

$$ \text{Ambient} = \text{effective color} \times k_a $$

Where:
*   $\text{effective color} = C_{\text{material}} \times C_{\text{light}}$ (the element-wise multiplication of the surface color and the light's intensity/color).
*   $k_a$ is the material's ambient coefficient (`material.ambient`).

### 2.2. Diffuse Reflection
Diffuse lighting represents the matte reflection of light that scatters uniformly in all directions (Lambertian reflection). The brightness depends on the angle of incidence between the light source ($\vec{L}$) and the surface normal ($\vec{N}$).

We calculate the cosine of the angle between $\vec{L}$ and $\vec{N}$ using the dot product:

$$ \text{light dot normal} = \vec{L} \cdot \vec{N} $$

*   **$\vec{L} \cdot \vec{N} < 0$**: The light source is behind the surface. In this case, both the diffuse and specular contributions are exactly **zero** (black).
*   **$\vec{L} \cdot \vec{N} \ge 0$**: The light is illuminating the front of the surface:

$$ \text{Diffuse} = \text{effective color} \times k_d \times (\vec{L} \cdot \vec{N}) $$

Where $k_d$ is the material's diffuse coefficient (`material.diffuse`).

### 2.3. Specular Reflection
Specular lighting represents the glossy, mirror-like highlight on shiny objects. The intensity depends on the alignment between the reflection vector ($\vec{R}$) and the eye vector ($\vec{V}$).

First, we calculate the reflection vector pointing away from the surface:

$$ \vec{R} = \text{reflect}(-\vec{L}, \vec{N}) $$

We then find the cosine of the angle between $\vec{R}$ and $\vec{V}$ using the dot product:

$$ \text{reflection dot eye} = \vec{R} \cdot \vec{V} $$

*   **$\vec{R} \cdot \vec{V} \le 0$**: The light reflects away from the observer's eye, resulting in **zero** specular highlight.
*   **$\vec{R} \cdot \vec{V} > 0$**: The highlight is visible to the observer. The contribution is calculated as:

$$ \text{Specular} = C_{\text{light}} \times k_s \times (\vec{R} \cdot \vec{V})^\alpha $$

Where:
*   $k_s$ is the material's specular coefficient (`material.specular`).
*   $\alpha$ is the material's shininess exponent (`material.shininess`). A higher shininess value concentrates the highlight into a smaller, sharper spot.

---

## 3. Code Mapping

In our C++ implementation (`src/Shading.cppm`), these mathematical concepts map directly to the code:

```cpp
Color lighting(Material material, PointLight light, Point point, Vector eye_vector, Vector normal_vector)
{
    // 1. Combine surface color with light intensity
    Color effective_color = material.color * light.intensity;

    // 2. Light vector (direction to light source)
    Vector light_vector = normalizeVector(light.position - point);

    // 3. Ambient component
    Color ambient = effective_color * material.ambient;

    // 4. Cosine of angle between light and normal
    double light_dot_normal = dotProduct(light_vector, normal_vector);

    Color diffuse(0.0, 0.0, 0.0);
    Color specular(0.0, 0.0, 0.0);

    if (light_dot_normal >= 0.0)
    {
        // 5. Diffuse component
        diffuse = effective_color * material.diffuse * light_dot_normal;

        // 6. Reflection vector & Cosine of angle between reflection and eye
        Vector reflect_vector = reflect(-light_vector, normal_vector);
        double reflect_dot_eye = dotProduct(reflect_vector, eye_vector);

        if (reflect_dot_eye > 0.0)
        {
            // 7. Specular component
            double factor = std::pow(reflect_dot_eye, material.shininess);
            specular = light.intensity * material.specular * factor;
        }
    }

    // 8. Sum all three components
    return ambient + diffuse + specular;
}
```
