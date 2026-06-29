//
// 4.SpherePhongReflection.cpp
// Showcases: Phong lighting model on a single sphere resting on an infinite floor plane.
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
    w.light = PointLight(Point(-5.0f, 8.0f, -6.0f), Color(1.0f, 1.0f, 1.0f));

    // Floor plane — matte warm white
    Plane floor;
    floor.material.color   = Color(0.95f, 0.93f, 0.88f);
    floor.material.specular = 0.0f;
    floor.material.diffuse  = 0.8f;
    w.add_plane(floor);

    // Single sphere — vibrant magenta with strong specular highlight
    Sphere s;
    s.set_transform(translation(0.0f, 1.0f, 0.0f));
    s.material.color     = Color(0.85f, 0.15f, 0.85f);
    s.material.diffuse   = 0.9f;
    s.material.specular  = 0.9f;
    s.material.shininess = 250.0f;
    w.add_sphere(s);

    // Camera — slightly elevated, looking at the sphere
    Camera cam(2560, 1440, PI / 3.0f);
    cam.set_transform(view_transform(
        Point(0.0f, 2.5f, -5.0f),
        Point(0.0f, 1.0f,  0.0f),
        Vector(0.0f, 1.0f,  0.0f)
    ));

    std::cout << "Rendering 4.SpherePhongReflection at 2560x1440..." << std::endl;
    Canvas image = render(cam, w);

    std::string ppmContent = canvasToPPM(image);
    savePPM("OutputSpherePhongReflection", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
