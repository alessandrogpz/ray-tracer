//
// 99.Showcase.cpp
// Showcases: One scene combining everything the ray tracer can do, used as the README's
// hero image. It always stays the last visualizer and grows with each new feature:
// Phong shading, hard shadows, matrix transformations, procedural patterns (checkers,
// gradient, rings, perturbed stripes), mirror reflection, glass with refraction blended
// by the Fresnel effect, and a shadow-free sky (casts_shadow).
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
    w.light = PointLight(Point(-5.0f, 9.0f, -7.0f), Color(1.0f, 0.98f, 0.94f));

    // ── Room ──────────────────────────────────────────────────────────────────

    // Floor — glossy checkers that reflect the scene
    Plane floor_plane;
    floor_plane.material.pattern    = checkers_pattern(Color(0.88f, 0.88f, 0.90f), Color(0.16f, 0.18f, 0.24f));
    floor_plane.material.specular   = 0.4f;
    floor_plane.material.reflective = 0.25f;
    w.add_plane(floor_plane);

    // Backdrop — subtle marble: wide, low-contrast stripes bent by Perlin noise
    Pattern backdrop_veins = stripe_pattern(Color(0.94f, 0.92f, 0.89f), Color(0.86f, 0.83f, 0.79f));
    backdrop_veins.set_transform(rotation_y(PI / 5.0f) * scale(1.2f, 1.2f, 1.2f));

    Plane backdrop;
    backdrop.set_transform(translation(0.0f, 0.0f, 9.0f) * rotation_x(PI / 2.0f));
    backdrop.material.pattern  = perturbed_pattern(backdrop_veins, 0.3f);
    backdrop.material.specular = 0.0f;
    w.add_plane(backdrop);

    // Wall behind the camera and a sky overhead — never seen directly, but they give the
    // mirror and glass a room to reflect instead of empty black space
    Plane rear_wall;
    rear_wall.set_transform(translation(0.0f, 0.0f, -14.0f) * rotation_x(PI / 2.0f));
    rear_wall.material.color    = Color(0.85f, 0.70f, 0.55f);
    rear_wall.material.specular = 0.0f;
    w.add_plane(rear_wall);

    Plane sky;
    sky.set_transform(translation(0.0f, 30.0f, 0.0f));
    sky.material.color        = Color(0.55f, 0.72f, 0.92f);
    sky.material.ambient      = 0.8f;
    sky.material.diffuse      = 0.2f;
    sky.material.specular     = 0.0f;
    sky.material.casts_shadow = false;
    w.add_plane(sky);

    // ── Spheres ───────────────────────────────────────────────────────────────

    // 1. Glass — refraction blended with reflection by the Fresnel effect (centre stage)
    Sphere glass = glass_sphere();
    glass.set_transform(translation(0.2f, 1.4f, 0.0f) * scale(1.4f, 1.4f, 1.4f));
    glass.material.color      = Color(0.02f, 0.03f, 0.04f);
    glass.material.diffuse    = 0.05f;
    glass.material.ambient    = 0.0f;
    glass.material.specular   = 1.0f;
    glass.material.shininess  = 300.0f;
    glass.material.reflective = 0.9f;
    w.add_sphere(glass);

    // 2. Mirror — a near-perfect reflector behind and to the left
    Sphere mirror;
    mirror.set_transform(translation(-3.1f, 1.2f, 2.4f) * scale(1.2f, 1.2f, 1.2f));
    mirror.material.color      = Color(0.06f, 0.06f, 0.08f);
    mirror.material.diffuse    = 0.1f;
    mirror.material.ambient    = 0.0f;
    mirror.material.specular   = 1.0f;
    mirror.material.shininess  = 300.0f;
    mirror.material.reflective = 0.92f;
    w.add_sphere(mirror);

    // 3. Marble — thin stripes perturbed by noise into veins, behind and to the right
    Pattern veins = stripe_pattern(Color(0.97f, 0.97f, 0.98f), Color(0.66f, 0.70f, 0.80f));
    veins.set_transform(rotation_z(PI / 3.0f) * scale(0.3f, 0.3f, 0.3f));
    Pattern marble = perturbed_pattern(veins, 0.4f);
    marble.set_transform(scale(0.5f, 0.5f, 0.5f));

    Sphere marble_ball;
    marble_ball.set_transform(translation(3.3f, 1.1f, 2.2f) * scale(1.1f, 1.1f, 1.1f));
    marble_ball.material.pattern    = marble;
    marble_ball.material.specular   = 0.8f;
    marble_ball.material.shininess  = 250.0f;
    marble_ball.material.reflective = 0.1f;
    w.add_sphere(marble_ball);

    // 4. Gradient — a coral-to-violet sunset, seen through and beside the glass
    Pattern sunset = gradient_pattern(Color(1.0f, 0.45f, 0.30f), Color(0.45f, 0.20f, 0.75f));
    sunset.set_transform(translation(-1.0f, 0.0f, 0.0f) * scale(2.0f, 1.0f, 1.0f));

    Sphere gradient_ball;
    gradient_ball.set_transform(translation(2.4f, 0.6f, -1.2f) * scale(0.6f, 0.6f, 0.6f));
    gradient_ball.material.pattern   = sunset;
    gradient_ball.material.specular  = 0.6f;
    gradient_ball.material.shininess = 150.0f;
    w.add_sphere(gradient_ball);

    // 5. Wood — a sphere squashed into a disc (transforms), with rings for grain
    Pattern grain = ring_pattern(Color(0.82f, 0.58f, 0.34f), Color(0.62f, 0.38f, 0.18f));
    grain.set_transform(scale(0.12f, 0.12f, 0.12f));

    Sphere wood_disc;
    wood_disc.set_transform(translation(-2.0f, 0.25f, -1.4f) * scale(0.85f, 0.25f, 0.85f));
    wood_disc.material.pattern   = grain;
    wood_disc.material.specular  = 0.3f;
    wood_disc.material.shininess = 60.0f;
    w.add_sphere(wood_disc);

    // ── Camera ────────────────────────────────────────────────────────────────
    Camera cam(2560, 1440, PI / 3.0f);
    cam.set_transform(view_transform(
        Point(0.0f,  3.2f, -8.0f),
        Point(0.0f,  1.0f,  0.6f),
        Vector(0.0f, 1.0f,  0.0f)
    ));

    std::cout << "Rendering 99.Showcase at 2560x1440..." << std::endl;
    Canvas image = render(cam, w);

    std::string ppmContent = canvasToPPM(image);
    savePPM("OutputShowcase", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
