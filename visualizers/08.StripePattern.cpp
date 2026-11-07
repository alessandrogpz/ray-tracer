//
// 08.StripePattern.cpp
// Showcases: Stripes in every form from Chapter 10, including its "Putting It Together"
// extensions: plain stripes that follow their sphere's transform, stripes with their own
// pattern transform, nested patterns (checkers whose squares hold perpendicular stripes,
// and stripes made of stripes), and perturbed stripes bent by Perlin noise into wavy veins.
//

import std;

import rt.tuple;
import rt.canvas;
import rt.colors;
import rt.matrix;
import rt.sphere;
import rt.plane;
import rt.lights;
import rt.materials;
import rt.patterns;
import rt.world;
import rt.camera;
import rt.transformations;

using namespace rt;

constexpr float PI = std::numbers::pi_v<float>;

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();

    World w;
    w.light = PointLight(Point(-6.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    // ── Planes ────────────────────────────────────────────────────────────────

    // Floor — nested: checkers whose squares hold stripes running in perpendicular directions
    Pattern floor_stripes_x = stripe_pattern(Color(0.92f, 0.92f, 0.94f), Color(0.72f, 0.74f, 0.80f));
    floor_stripes_x.set_transform(scale(0.25f, 0.25f, 0.25f));
    Pattern floor_stripes_z = stripe_pattern(Color(0.30f, 0.34f, 0.44f), Color(0.20f, 0.23f, 0.31f));
    floor_stripes_z.set_transform(rotation_y(PI / 2.0f) * scale(0.25f, 0.25f, 0.25f));

    Plane floor_plane;
    floor_plane.material.pattern  = checkers_pattern(floor_stripes_x, floor_stripes_z);
    floor_plane.material.specular = 0.1f;
    w.add_plane(floor_plane);

    // Back wall — perturbed: wide warm stripes jittered by noise into wavy bands
    Pattern wall_stripes = stripe_pattern(Color(0.96f, 0.89f, 0.78f), Color(0.85f, 0.60f, 0.45f));
    wall_stripes.set_transform(scale(0.6f, 0.6f, 0.6f));

    Plane back_wall;
    back_wall.set_transform(translation(0.0f, 0.0f, 10.0f) * rotation_x(PI / 2.0f));
    back_wall.material.pattern  = perturbed_pattern(wall_stripes, 0.35f);
    back_wall.material.specular = 0.0f;
    w.add_plane(back_wall);

    // ── Spheres — one kind of stripe each ─────────────────────────────────────

    // 1. Plain stripes — the pattern simply follows the sphere's transform
    Sphere plain;
    plain.set_transform(translation(-4.6f, 0.7f, 1.2f) * scale(0.7f, 0.7f, 0.7f));
    plain.material.pattern   = stripe_pattern(Color(0.95f, 0.25f, 0.30f), Color(1.0f, 1.0f, 1.0f));
    plain.material.specular  = 0.6f;
    plain.material.shininess = 150.0f;
    w.add_sphere(plain);

    // 2. Transformed stripes — the pattern's own transform makes them thin and diagonal
    Pattern candy = stripe_pattern(Color(0.10f, 0.70f, 0.65f), Color(1.0f, 1.0f, 1.0f));
    candy.set_transform(rotation_z(PI / 4.0f) * scale(0.18f, 0.18f, 0.18f));

    Sphere transformed;
    transformed.set_transform(translation(-2.2f, 1.0f, 0.4f));
    transformed.material.pattern   = candy;
    transformed.material.specular  = 0.7f;
    transformed.material.shininess = 200.0f;
    w.add_sphere(transformed);

    // 3. Perturbed stripes — Perlin noise bends straight stripes into wavy, marble-like veins
    Pattern veins = stripe_pattern(Color(0.95f, 0.95f, 0.97f), Color(0.45f, 0.50f, 0.62f));
    veins.set_transform(rotation_z(PI / 3.0f) * scale(0.35f, 0.35f, 0.35f));
    Pattern marble = perturbed_pattern(veins, 0.45f);
    marble.set_transform(scale(0.5f, 0.5f, 0.5f));

    Sphere perturbed;
    perturbed.set_transform(translation(0.8f, 1.5f, 0.6f) * scale(1.5f, 1.5f, 1.5f));
    perturbed.material.pattern   = marble;
    perturbed.material.specular  = 0.9f;
    perturbed.material.shininess = 300.0f;
    w.add_sphere(perturbed);

    // 4. Nested stripes — wide stripes whose bands are themselves finer stripes
    Pattern fine_gold = stripe_pattern(Color(1.0f, 0.75f, 0.20f), Color(0.85f, 0.50f, 0.10f));
    fine_gold.set_transform(rotation_z(PI / 2.0f) * scale(0.15f, 0.15f, 0.15f));
    Pattern fine_violet = stripe_pattern(Color(0.45f, 0.25f, 0.65f), Color(0.30f, 0.15f, 0.45f));
    fine_violet.set_transform(scale(0.15f, 0.15f, 0.15f));
    Pattern banded = stripe_pattern(fine_gold, fine_violet);
    banded.set_transform(scale(0.5f, 0.5f, 0.5f));

    Sphere nested;
    nested.set_transform(translation(4.0f, 0.9f, 0.0f) * scale(0.9f, 0.9f, 0.9f));
    nested.material.pattern   = banded;
    nested.material.specular  = 0.5f;
    nested.material.shininess = 100.0f;
    w.add_sphere(nested);

    // ── Camera ────────────────────────────────────────────────────────────────
    Camera cam(2560, 1440, PI / 3.0f);
    cam.set_transform(view_transform(
        Point(0.0f,  3.0f, -9.0f),
        Point(0.0f,  1.0f,  0.0f),
        Vector(0.0f, 1.0f,  0.0f)
    ));

    std::cout << "Rendering 08.StripePattern at 2560x1440..." << std::endl;
    Canvas image = render(cam, w);

    std::string ppmContent = canvasToPPM(image);
    savePPM("OutputStripePattern", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
