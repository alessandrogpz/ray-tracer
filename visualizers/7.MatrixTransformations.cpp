//
// 7.MatrixTransformations.cpp
// Showcases: Multiple spheres each transformed independently with translation,
// scaling, rotation, and combined transforms — demonstrating the full power of
// 4x4 matrix transformations in a single QHD scene.
//

#include <string>
#include <chrono>
#include <iostream>
#include <numbers>

import rt.tuple;
import rt.canvas;
import rt.colors;
import rt.matrix;
import rt.sphere;
import rt.plane;
import rt.ray;
import rt.lights;
import rt.materials;
import rt.world;
import rt.camera;
import rt.transformations;

using namespace rt;

constexpr float PI = std::numbers::pi_v<float>;

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();

    World w;
    // Warm key light above-left, cooler fill light above-right
    w.light = PointLight(Point(-8.0f, 12.0f, -8.0f), Color(1.0f, 0.97f, 0.90f));

    // ── Planes ────────────────────────────────────────────────────────────────

    // Floor — cool-tinted off-white
    Plane floor_plane;
    floor_plane.material.color   = Color(0.90f, 0.91f, 0.95f);
    floor_plane.material.specular = 0.1f;
    w.add_plane(floor_plane);

    // Back wall — slightly warm, rotated 90° around X so it faces forward
    Plane back_wall;
    back_wall.set_transform(translation(0.0f, 0.0f, 8.0f) * rotation_x(PI / 2.0f));
    back_wall.material.color   = Color(0.88f, 0.88f, 0.92f);
    back_wall.material.specular = 0.0f;
    w.add_plane(back_wall);

    // Left wall — angled 45°
    Plane left_wall;
    left_wall.set_transform(
        translation(0.0f, 0.0f, 8.0f) *
        rotation_y(-PI / 4.0f) *
        rotation_x( PI / 2.0f)
    );
    left_wall.material.color   = Color(0.86f, 0.88f, 0.93f);
    left_wall.material.specular = 0.0f;
    w.add_plane(left_wall);

    // Right wall — angled 45° the other way
    Plane right_wall;
    right_wall.set_transform(
        translation(0.0f, 0.0f, 8.0f) *
        rotation_y( PI / 4.0f) *
        rotation_x( PI / 2.0f)
    );
    right_wall.material.color   = Color(0.86f, 0.88f, 0.93f);
    right_wall.material.specular = 0.0f;
    w.add_plane(right_wall);

    // ── Spheres — one transform type each ────────────────────────────────────

    // 1. Translation only — coral sphere placed to the far left on the floor
    Sphere s_translation;
    s_translation.set_transform(translation(-4.5f, 1.0f, 1.0f));
    s_translation.material.color    = Color(0.95f, 0.35f, 0.30f); // Coral red
    s_translation.material.diffuse  = 0.85f;
    s_translation.material.specular = 0.6f;
    s_translation.material.shininess = 120.0f;
    w.add_sphere(s_translation);

    // 2. Uniform scale — teal sphere, larger than unit
    Sphere s_scale_up;
    s_scale_up.set_transform(translation(-1.8f, 1.5f, 0.5f) * scale(1.5f, 1.5f, 1.5f));
    s_scale_up.material.color    = Color(0.15f, 0.78f, 0.72f); // Teal
    s_scale_up.material.diffuse  = 0.85f;
    s_scale_up.material.specular = 0.7f;
    s_scale_up.material.shininess = 180.0f;
    w.add_sphere(s_scale_up);

    // 3. Non-uniform scale — amber sphere, squashed into an oblate disc
    Sphere s_scale_nonuniform;
    s_scale_nonuniform.set_transform(translation(1.0f, 0.4f, -0.5f) * scale(1.3f, 0.4f, 1.3f));
    s_scale_nonuniform.material.color    = Color(1.0f, 0.68f, 0.10f); // Amber
    s_scale_nonuniform.material.diffuse  = 0.8f;
    s_scale_nonuniform.material.specular = 0.5f;
    s_scale_nonuniform.material.shininess = 80.0f;
    w.add_sphere(s_scale_nonuniform);

    // 4. Rotation only (rotated around Z, elongated on Y so shape is visible) — violet
    Sphere s_rotation;
    s_rotation.set_transform(
        translation(3.2f, 0.8f, 0.0f) *
        rotation_z(PI / 4.0f) *
        scale(0.4f, 1.2f, 0.4f)   // elongated to make rotation apparent
    );
    s_rotation.material.color    = Color(0.60f, 0.20f, 0.90f); // Violet
    s_rotation.material.diffuse  = 0.85f;
    s_rotation.material.specular = 0.8f;
    s_rotation.material.shininess = 200.0f;
    w.add_sphere(s_rotation);

    // 5. Combined (translate + rotate + scale) — small electric-blue sphere
    //    sitting on top of the teal one, rotated and scaled small
    Sphere s_combined;
    s_combined.set_transform(
        translation(-1.8f, 3.5f, 0.5f) *
        rotation_y(PI / 5.0f) *
        scale(0.35f, 0.35f, 0.35f)
    );
    s_combined.material.color    = Color(0.20f, 0.55f, 1.0f); // Electric blue
    s_combined.material.diffuse  = 0.8f;
    s_combined.material.specular = 0.95f;
    s_combined.material.shininess = 300.0f;
    w.add_sphere(s_combined);

    // 6. Scale down only — tiny orange sphere, far right
    Sphere s_scale_down;
    s_scale_down.set_transform(translation(5.0f, 0.3f, -0.5f) * scale(0.3f, 0.3f, 0.3f));
    s_scale_down.material.color    = Color(1.0f, 0.50f, 0.05f); // Orange
    s_scale_down.material.diffuse  = 0.8f;
    s_scale_down.material.specular = 0.7f;
    s_scale_down.material.shininess = 150.0f;
    w.add_sphere(s_scale_down);

    // ── Camera ────────────────────────────────────────────────────────────────
    // Wide angle, elevated slightly, looking at the centre of the sphere lineup
    Camera cam(2560, 1440, PI / 2.8f);
    cam.set_transform(view_transform(
        Point(0.0f,  3.5f, -9.0f),
        Point(0.0f,  1.2f,  0.0f),
        Vector(0.0f, 1.0f,  0.0f)
    ));

    std::cout << "Rendering 7.MatrixTransformations at 2560x1440..." << std::endl;
    Canvas image = render(cam, w);

    std::string ppmContent = canvasToPPM(image);
    savePPM("OutputMatrixTransformations", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
