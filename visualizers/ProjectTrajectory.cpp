#include <string>

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
    point position;
    vector velocity;

    projectile() : position(createPoint(0, 0, 0)), velocity(createVector(0, 0, 0)) {}
    projectile(point p, vector v) : position(p), velocity(v) {}
};

struct environment {
    vector gravity;
    vector wind;

    environment(vector g, vector w) : gravity(g), wind(w) {}
};

projectile createProjectile(point position, vector velocity)
{
    return {position, velocity};
}

environment createEnvironment(vector wind, vector gravity)
{
    return {gravity, wind};
}

void writePixelBlock(canvas &c, int h, int w, int startX, int startY, color col) {
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
    canvas c(1000, 1000);
    color green(0, 1, 0);

    vector direction = createVector(1.0, 1.5, 0.0);
    vector unit_direction = normalizeVector(direction);

    double speed = 11.25;
    vector velocity = unit_direction * speed;

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

    return 0;
}