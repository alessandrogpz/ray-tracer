export module rt.materials;

import rt.colors;

export namespace rt
{
    struct material
    {
        Color material_color{};
        double ambient{};
        double diffuse{};
        double specular{};
        double shininess{};
    };
}