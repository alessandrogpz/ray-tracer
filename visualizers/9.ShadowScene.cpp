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
    // Light positioned to cast a shadow from the large sphere onto the small sphere
    w.light = PointLight(Point(-3.0f, 3.0f, -3.0f), Color(1.0f, 1.0f, 1.0f));

    // Large central sphere (unscaled)
    Sphere large_sphere = Sphere();
    large_sphere.material.color = Color(0.2f, 0.5f, 0.9f); // Blue
    large_sphere.material.diffuse = 0.7f;
    large_sphere.material.specular = 0.3f;
    w.add_sphere(large_sphere);

    // Small sphere (uniformly scaled down to 0.5 and translated to the right/back)
    // Uniform scaling (equal scale along all axes) does not cause normal distortion or large float precision drift
    Sphere small_sphere = Sphere();
    small_sphere.set_transform(translation(1.5f, 0.0f, 1.5f) * scale(0.5f, 0.5f, 0.5f));
    small_sphere.material.color = Color(1.0f, 0.2f, 0.2f); // Red
    small_sphere.material.diffuse = 0.7f;
    small_sphere.material.specular = 0.3f;
    w.add_sphere(small_sphere);

    // Camera setup (hsize=800, vsize=600, fov=pi/3)
    Camera cam(800, 600, PI / 3.0f);
    cam.set_transform(view_transform(Point(0.0f, 1.5f, -5.0f),
                                     Point(0.5f, 0.0f, 0.0f),
                                     Vector(0.0f, 1.0f, 0.0f)));

    // Render the scene
    std::cout << "Rendering standard shadow scene..." << std::endl;
    Canvas image = render(cam, w);

    // Save output to PPM
    std::string ppmContent = canvasToPPM(image);
    savePPM("OutputShadowScene", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
