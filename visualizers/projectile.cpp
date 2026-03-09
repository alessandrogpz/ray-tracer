#include "raylib.h"
#include "../includes/Tuple.hpp"
#include <vector>

/**
 * A Projectile has a position (Point) and a velocity (Vector).
 * An Environment has wind (Vector) and gravity (Vector).
 *
 * The tick(environment, projectile) function returns a new projectile
 * representing the updated state after a single time step (tick).
 * 
 * Compilation
 * g++ projectile.cpp ../src/Tuple.cpp -o my_viz -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
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

projectile tick(environment env, projectile proj)
{
    // If you want smooth movement regardless of FPS:
    // New Pos = Old Pos + (Velocity * dt)
    // New Vel = Old Vel + (Gravity + Wind) * dt
    
    projectile updated;
    updated.position = addTuples(proj.position, proj.velocity);
    
    tuple acceleration = addTuples(env.gravity, env.wind);
    updated.velocity = addTuples(proj.velocity, acceleration);

    return updated;
}

int main() {
    InitWindow(1200, 800, "Bullet Trajectory");

    tuple startPos = createPoint(50, 400, 0);
    tuple startVel = createVector(20, -20, 0);
    projectile proj = createProjectile(startPos, startVel);

    tuple gravity = createVector(0, 1, 0);        // Gravity points DOWN (positive Y in Raylib)
    tuple wind = createVector(-0.1, 0, 0);
    environment env = createEnvironment(gravity, wind);
    
    std::vector<Vector2> path;
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        proj = tick(env, proj);

        // Convert tuple to Raylib's Vector2 for drawing
        Vector2 currentPos = { proj.position.x, proj.position.y };

        if (currentPos.y < 800 && currentPos.x < 1200) {
            path.push_back(currentPos);
        }

        BeginDrawing();
            ClearBackground(BLACK);

            // Draw Trail
            for (size_t i = 1; i < path.size(); i++) {
                DrawLineV(path[i-1], path[i], MAROON);
            }

            // Draw Bullet
            DrawCircleV(currentPos, 10, RAYWHITE);

            DrawText("R to Reset", 10, 10, 20, LIGHTGRAY);
            DrawText("ESC to Quit", 10, 30, 20, LIGHTGRAY);
            if (IsKeyPressed(KEY_R)) {
                proj = createProjectile(startPos, startVel);
                path.clear();
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}