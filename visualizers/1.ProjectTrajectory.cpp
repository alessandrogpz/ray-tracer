#include <string>
#include <chrono>
#include <iostream>

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;

using namespace rt;

/**
 * A Projectile has a position (Point) and a velocity (Vector).
 * An Environment has wind (Vector) and gravity (Vector).
 *
 * The tick(environment, projectile) function returns a new projectile
 * representing the updated state after a single time step (tick).
 * 
 * build:
 * g++ -std=c++20 -fmodules-ts ../src/Utils.cppm ../src/Tuple.cppm ../src/Colors.cppm ../src/Canvas.cppm ../src/Matrix.cppm ../src/Transformations.cppm project_trajectory.cpp -o project_trajectory
 */

struct projectile {
    Point position;
    Vector velocity;

    projectile() : position(createPoint(0, 0, 0)), velocity(createVector(0, 0, 0)) {}
    projectile(Point p, Vector v) : position(p), velocity(v) {}
};

struct environment {
    Vector gravity;
    Vector wind;

    environment(Vector g, Vector w) : gravity(g), wind(w) {}
};

projectile createProjectile(Point position, Vector velocity)
{
    return {position, velocity};
}

environment createEnvironment(Vector wind, Vector gravity)
{
    return {gravity, wind};
}

void writePixelBlock(Canvas &c, int h, int w, int startX, int startY, Color col) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            size_t drawX = startX + j;
            size_t drawY = startY + i;
            if (drawX < c.width && drawY < c.height) {
                writePixel(c, drawX, drawY, col);
            }
        }
    }
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    Canvas c(1000, 1000);
    Color green(0, 1, 0);

    Vector direction = createVector(1.0, 1.5, 0.0);
    Vector unit_direction = normalizeVector(direction);

    double speed = 11.25;
    Vector velocity = unit_direction * speed;

    projectile proj(createPoint(0.0, 1.0, 0.0), velocity);
    environment env(createVector(0.0, -0.1, 0.0), createVector(-0.01, 0.0, 0.0));

    while (proj.position.y > 0) {
        int x = static_cast<int>(proj.position.x);
        int y = 1000 - static_cast<int>(proj.position.y);

        if (x >= 0 && x < 1000 && y >= 0 && y < 1000) {
            writePixelBlock(c, 5, 5, x, y, green);
        }

        proj.position = proj.position + proj.velocity;
        proj.velocity = proj.velocity + env.gravity + env.wind;
    }

    std::string ppmContent = canvasToPPM(c);
    savePPM("OutputProjectTrajectory.ppm", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}