#include "../includes/Tuple.hpp"
#include "../includes/Colors.hpp"
#include "../includes/Canvas.hpp"
#include <vector>

/**
 * A Projectile has a position (Point) and a velocity (Vector).
 * An Environment has wind (Vector) and gravity (Vector).
 *
 * The tick(environment, projectile) function returns a new projectile
 * representing the updated state after a single time step (tick).
 * 
 * build:
 * g++ -std=c++20 project_trajectory.cpp ../src/Tuple.cpp ../src/Canvas.cpp ../src/Colors.cpp -o buid
 */

struct projectile {
    tuple position;
    tuple velocity;
    
    projectile() : position(0,0,0,1), velocity(0,0,0,0) {} // Default
    projectile(tuple p, tuple v) : position(p), velocity(v) {}
};

struct environment {
    tuple gravity;
    tuple wind;
    
    environment(tuple g, tuple w) : gravity(g), wind(w) {}
};

projectile createProjectile(tuple position, tuple velocity)
{
    projectile newProjectile;
    newProjectile.position = position;
    newProjectile.velocity = velocity;
    return newProjectile;
}

environment createEnvironment(tuple wind, tuple gravity)
{
    return environment(gravity, wind);
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

    // Create a raw direction vector
    tuple direction = createVector(1, 1.5, 0);

    // Normalize it using your standalone function
    tuple unit_direction = normalizeVector(direction);

    // Scale it by your desired speed
    float speed = 11.25f;
    tuple velocity = unit_direction * speed;

    // Final projectile initialization
    projectile proj(createPoint(0, 1, 0), velocity);

    environment env(tuple(0, -0.1, 0, 0), tuple(-0.01, 0, 0, 0));

    // Run until the projectile hits the ground (y <= 0)
    while (proj.position.y > 0) {
        // Map world coordinates to canvas coordinates
        int x = static_cast<int>(proj.position.x);
        int y = 1000 - static_cast<int>(proj.position.y);

        // Check bounds before drawing
        if (x >= 0 && x < 1000 && y >= 0 && y < 1000) {
            writePixelBlock(c, 5, 5, x, y, green);
        }

        // Update Physics (Euler Integration)
        proj.position = proj.position + proj.velocity;
        proj.velocity = proj.velocity + env.gravity + env.wind;
    }

    std::string ppmContent = canvasToPPM(c);
    savePPM("project_trajectory_output.ppm", ppmContent);

    return 0;
}