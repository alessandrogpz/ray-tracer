//
// 5.MultipleSpherePhongReflections.cpp
// Showcases: Phong lighting on multiple spheres with different materials resting on an infinite floor plane.
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
    w.light = PointLight(Point(-8.0f, 10.0f, -8.0f), Color(1.0f, 1.0f, 1.0f));

    // Floor plane — matte light gray
    Plane floor;
    floor.material.color    = Color(0.92f, 0.92f, 0.90f);
    floor.material.specular = 0.0f;
    floor.material.diffuse  = 0.8f;
    w.add_plane(floor);

    // Centre sphere — large, vibrant magenta
    Sphere s1;
    s1.set_transform(translation(0.0f, 1.0f, 0.5f));
    s1.material.color     = Color(0.85f, 0.15f, 0.85f);
    s1.material.diffuse   = 0.9f;
    s1.material.specular  = 0.9f;
    s1.material.shininess = 200.0f;
    w.add_sphere(s1);

    // Right sphere — cool blue, half size
    Sphere s2;
    s2.set_transform(translation(1.6f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f));
    s2.material.color     = Color(0.1f, 0.55f, 1.0f);
    s2.material.diffuse   = 0.9f;
    s2.material.specular  = 0.9f;
    s2.material.shininess = 200.0f;
    w.add_sphere(s2);

    // Left sphere — warm yellow-orange, slightly smaller
    Sphere s3;
    s3.set_transform(translation(-1.6f, 0.65f, -0.75f) * scale(0.65f, 0.65f, 0.65f));
    s3.material.color     = Color(1.0f, 0.72f, 0.1f);
    s3.material.diffuse   = 0.9f;
    s3.material.specular  = 0.9f;
    s3.material.shininess = 200.0f;
    w.add_sphere(s3);

    // Camera — slightly elevated, looking toward the centre sphere
    Camera cam(2560, 1440, PI / 3.0f);
    cam.set_transform(view_transform(
        Point(0.0f, 2.5f, -6.0f),
        Point(0.0f, 1.0f,  0.0f),
        Vector(0.0f, 1.0f,  0.0f)
    ));

    std::cout << "Rendering 5.MultipleSpherePhongReflections at 2560x1440..." << std::endl;
    Canvas image = render(cam, w);

    std::string ppmContent = canvasToPPM(image);
    savePPM("OutputMultipleSpherePhongReflections", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
