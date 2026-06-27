//
// Created by aper on 27.06.26.
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

    // Create the world
    World w;
    w.light = PointLight(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    // Floor (extremely flattened sphere scaled by (10, 0.01, 10))
    Sphere floor_sphere = Sphere();
    floor_sphere.set_transform(scale(10.0f, 0.01f, 10.0f));
    floor_sphere.material.color = Color(1.0f, 0.9f, 0.9f);
    floor_sphere.material.specular = 0.0f;
    w.add_sphere(floor_sphere);

    // Left wall (same material, rotated and translated)
    Sphere left_wall = Sphere();
    left_wall.set_transform(translation(0.0f, 0.0f, 5.0f) *
                           rotation_y(-PI / 4.0f) *
                           rotation_x(PI / 2.0f) *
                           scale(10.0f, 0.01f, 10.0f));
    left_wall.material = floor_sphere.material;
    w.add_sphere(left_wall);

    // Right wall
    Sphere right_wall = Sphere();
    right_wall.set_transform(translation(0.0f, 0.0f, 5.0f) *
                            rotation_y(PI / 4.0f) *
                            rotation_x(PI / 2.0f) *
                            scale(10.0f, 0.01f, 10.0f));
    right_wall.material = floor_sphere.material;
    w.add_sphere(right_wall);

    // Middle sphere (large green sphere)
    Sphere middle = Sphere();
    middle.set_transform(translation(-0.5f, 1.0f, 0.5f));
    middle.material.color = Color(0.1f, 1.0f, 0.5f);
    middle.material.diffuse = 0.7f;
    middle.material.specular = 0.3f;
    w.add_sphere(middle);

    // Right sphere (half sized, translated right)
    Sphere right = Sphere();
    right.set_transform(translation(1.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f));
    right.material.color = Color(0.5f, 1.0f, 0.1f);
    right.material.diffuse = 0.7f;
    right.material.specular = 0.3f;
    w.add_sphere(right);

    // Left sphere (third sized, translated left)
    Sphere left = Sphere();
    left.set_transform(translation(-1.5f, 0.33f, -0.75f) * scale(0.33f, 0.33f, 0.33f));
    left.material.color = Color(1.0f, 0.8f, 0.1f);
    left.material.diffuse = 0.7f;
    left.material.specular = 0.3f;
    w.add_sphere(left);

    // Camera setup (hsize=2560, vsize=1440, fov=pi/3)
    Camera cam(2560, 1440, PI / 3.0f);
    cam.set_transform(view_transform(Point(0.0f, 1.5f, -5.0f),
                                     Point(0.0f, 1.0f, 0.0f),
                                     Vector(0.0f, 1.0f, 0.0f)));

    // Render the scene
    std::cout << "Rendering scene at 2560x1440 resolution..." << std::endl;
    Canvas image = render(cam, w);

    // Save output to PPM
    std::string ppmContent = canvasToPPM(image);
    savePPM("OutputFirstScene", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
