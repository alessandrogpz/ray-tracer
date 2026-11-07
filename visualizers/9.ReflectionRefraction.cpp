//
// 9.ReflectionRefraction.cpp
// Showcases: Chapter 11 materials — a glossy reflective floor, a mirror sphere,
// a glass sphere with an air bubble (Fresnel-blended reflection and refraction),
// and tinted glass, all reflecting and refracting a patterned room.
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
    w.light = PointLight(Point(-4.0f, 9.0f, -8.0f), Color(1.0f, 1.0f, 1.0f));

    // ── Room ──────────────────────────────────────────────────────────────────

    // Floor — checkers with a subtle glossy reflection
    Plane floor_plane;
    floor_plane.material.pattern    = checkers_pattern(Color(0.85f, 0.85f, 0.88f), Color(0.10f, 0.12f, 0.18f));
    floor_plane.material.reflective = 0.3f;
    floor_plane.material.specular   = 0.3f;
    w.add_plane(floor_plane);

    // Back wall — vertical stripes, which make the glass's distortion easy to see
    Plane back_wall;
    back_wall.set_transform(translation(0.0f, 0.0f, 8.0f) * rotation_x(PI / 2.0f));
    Pattern wall_stripes = stripe_pattern(Color(0.95f, 0.80f, 0.55f), Color(0.80f, 0.35f, 0.30f));
    wall_stripes.set_transform(scale(0.5f, 0.5f, 0.5f));
    back_wall.material.pattern  = wall_stripes;
    back_wall.material.specular = 0.0f;
    w.add_plane(back_wall);

    // Left wall — rings
    Plane left_wall;
    left_wall.set_transform(translation(-8.0f, 0.0f, 0.0f) * rotation_z(PI / 2.0f));
    left_wall.material.pattern  = ring_pattern(Color(0.40f, 0.60f, 0.85f), Color(0.85f, 0.90f, 0.95f));
    left_wall.material.specular = 0.0f;
    w.add_plane(left_wall);

    // ── Spheres ───────────────────────────────────────────────────────────────

    // 1. Mirror — dark, almost perfectly reflective sphere
    Sphere mirror;
    mirror.set_transform(translation(-2.6f, 1.2f, 1.5f) * scale(1.2f, 1.2f, 1.2f));
    mirror.material.color      = Color(0.05f, 0.05f, 0.08f);
    mirror.material.diffuse    = 0.1f;
    mirror.material.ambient    = 0.0f;
    mirror.material.specular   = 1.0f;
    mirror.material.shininess  = 300.0f;
    mirror.material.reflective = 0.95f;
    w.add_sphere(mirror);

    // 2. Glass sphere with an air bubble in the middle. Pure refraction would leave a black
    //    band where total internal reflection occurs; the Fresnel (Schlick) blend fills it in.
    Sphere glass = glass_sphere();
    glass.set_transform(translation(0.4f, 1.3f, -0.4f) * scale(1.3f, 1.3f, 1.3f));
    glass.material.color      = Color(0.02f, 0.02f, 0.02f);
    glass.material.diffuse    = 0.05f;
    glass.material.ambient    = 0.0f;
    glass.material.specular   = 1.0f;
    glass.material.shininess  = 300.0f;
    glass.material.reflective = 0.9f;
    w.add_sphere(glass);

    Sphere bubble = glass_sphere();
    bubble.set_transform(translation(0.4f, 1.3f, -0.4f) * scale(0.6f, 0.6f, 0.6f));
    bubble.material.color            = Color(0.0f, 0.0f, 0.0f);
    bubble.material.diffuse          = 0.0f;
    bubble.material.ambient          = 0.0f;
    bubble.material.specular         = 1.0f;
    bubble.material.shininess        = 300.0f;
    bubble.material.reflective       = 0.9f;
    bubble.material.refractive_index = 1.00029f; // Air
    w.add_sphere(bubble);

    // 3. Tinted glass — a very dark red reads as red glass (see the book's tips)
    Sphere tinted = glass_sphere();
    tinted.set_transform(translation(2.8f, 0.7f, -1.4f) * scale(0.7f, 0.7f, 0.7f));
    tinted.material.color      = Color(0.25f, 0.0f, 0.02f);
    tinted.material.diffuse    = 0.2f;
    tinted.material.ambient    = 0.05f;
    tinted.material.specular   = 1.0f;
    tinted.material.shininess  = 300.0f;
    tinted.material.reflective = 0.9f;
    w.add_sphere(tinted);

    // 4. Solid colored spheres behind the glass, to show refraction bending them
    Sphere green_ball;
    green_ball.set_transform(translation(1.6f, 0.6f, 3.0f) * scale(0.6f, 0.6f, 0.6f));
    green_ball.material.color    = Color(0.20f, 0.80f, 0.35f);
    green_ball.material.specular = 0.5f;
    w.add_sphere(green_ball);

    Sphere blue_ball;
    blue_ball.set_transform(translation(-0.8f, 0.5f, 3.6f) * scale(0.5f, 0.5f, 0.5f));
    blue_ball.material.color    = Color(0.20f, 0.45f, 0.95f);
    blue_ball.material.specular = 0.5f;
    w.add_sphere(blue_ball);

    // ── Camera ────────────────────────────────────────────────────────────────
    Camera cam(2560, 1440, PI / 3.0f);
    cam.set_transform(view_transform(
        Point(0.5f,  2.5f, -7.5f),
        Point(0.2f,  1.0f,  0.0f),
        Vector(0.0f, 1.0f,  0.0f)
    ));

    std::cout << "Rendering 9.ReflectionRefraction at 2560x1440..." << std::endl;
    Canvas image = render(cam, w);

    std::string ppmContent = canvasToPPM(image);
    savePPM("OutputReflectionRefraction", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
