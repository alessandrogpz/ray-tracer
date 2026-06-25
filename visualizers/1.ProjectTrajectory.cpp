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
    constexpr size_t CANVAS_WIDTH = 2560;
    constexpr size_t CANVAS_HEIGHT = 1440;
    Canvas c(CANVAS_WIDTH, CANVAS_HEIGHT);
    Color green(0, 1, 0);

    Vector direction = createVector(1.0, 1.5, 0.0);
    Vector unit_direction = normalizeVector(direction);

    double speed = 11.25;
    Vector velocity = unit_direction * speed;

    projectile proj(createPoint(0.0, 1.0, 0.0), velocity);
    environment env(createVector(0.0, -0.1, 0.0), createVector(-0.01, 0.0, 0.0));

    while (proj.position.y > 0) {
        int x = static_cast<int>(proj.position.x * (static_cast<double>(CANVAS_WIDTH) / 1000.0));
        int y = static_cast<int>(static_cast<double>(CANVAS_HEIGHT) - proj.position.y * (static_cast<double>(CANVAS_HEIGHT) / 1000.0));

        if (x >= 0 && x < static_cast<int>(CANVAS_WIDTH) && y >= 0 && y < static_cast<int>(CANVAS_HEIGHT)) {
            writePixelBlock(c, 8, 8, x, y, green);
        }

        proj.position = proj.position + proj.velocity;
        proj.velocity = proj.velocity + env.gravity + env.wind;
    }

    std::string ppmContent = canvasToPPM(c);
    savePPM("OutputProjectTrajectory", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}