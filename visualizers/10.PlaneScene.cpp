//
// Created by aper on 28.06.26.
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
    // Strong point light source
    w.light = PointLight(Point(-5.0f, 8.0f, -5.0f), Color(1.0f, 1.0f, 1.0f));

    // 1. Floor Plane (infinite plane at y = 0)
    Plane floor = Plane();
    floor.material.color = Color(0.9f, 0.9f, 0.85f); // Light warm gray
    floor.material.specular = 0.0f;                  // Matte
    floor.material.diffuse = 0.7f;
    w.add_plane(floor);

    // 2. Backdrop Plane (infinite plane at z = 10, rotated perpendicular to Y)
    Plane back_wall = Plane();
    back_wall.set_transform(translation(0.0f, 0.0f, 10.0f) * rotation_x(PI / 2.0f));
    back_wall.material.color = Color(0.15f, 0.15f, 0.25f); // Deep blue
    back_wall.material.specular = 0.1f;
    back_wall.material.diffuse = 0.6f;
    w.add_plane(back_wall);

    // 3. Middle Sphere (large green sphere)
    Sphere middle = Sphere();
    middle.set_transform(translation(0.0f, 1.0f, 2.0f));
    middle.material.color = Color(0.1f, 0.9f, 0.4f);
    middle.material.diffuse = 0.7f;
    middle.material.specular = 0.3f;
    middle.material.shininess = 200.0f;
    w.add_sphere(middle);

    // 4. Left Sphere (medium orange sphere)
    Sphere left = Sphere();
    left.set_transform(translation(-2.0f, 0.75f, 1.0f) * scale(0.75f, 0.75f, 0.75f));
    left.material.color = Color(0.9f, 0.4f, 0.1f);
    left.material.diffuse = 0.7f;
    left.material.specular = 0.3f;
    left.material.shininess = 200.0f;
    w.add_sphere(left);

    // 5. Right Sphere (small blue sphere)
    Sphere right = Sphere();
    right.set_transform(translation(2.0f, 0.5f, 1.5f) * scale(0.5f, 0.5f, 0.5f));
    right.material.color = Color(0.1f, 0.5f, 0.9f);
    right.material.diffuse = 0.7f;
    right.material.specular = 0.3f;
    right.material.shininess = 200.0f;
    w.add_sphere(right);

    // Camera setup (hsize=1920, vsize=1080, fov=pi/3)
    Camera cam(1920, 1080, PI / 3.0f);
    cam.set_transform(view_transform(Point(0.0f, 3.0f, -6.0f),
                                     Point(0.0f, 1.0f, 1.0f),
                                     Vector(0.0f, 1.0f, 0.0f)));

    // Render the scene
    std::cout << "Rendering scene with infinite planes at 1920x1080 resolution..." << std::endl;
    Canvas image = render(cam, w);

    // Save output to PPM
    std::string ppmContent = canvasToPPM(image);
    savePPM("OutputPlaneScene", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
