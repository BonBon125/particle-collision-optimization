#include "ParticleSystem.h"
#include "Constants.h"
#include "Utils.h"
#include <algorithm>
#include <cmath>
ParticleSystem::ParticleSystem(std::string methodName)
    : mCollisionHandlingMethod(methodName)
{
    for (int i = 0; i < NUM_PARTICLES; i++) {
        mParticles[i] = new Particle();
        mParticles[i]->x = RandomGen::get(WORLD_MIN, WORLD_MAX);
        mParticles[i]->y = RandomGen::get(WORLD_MIN, WORLD_MAX);
        mParticles[i]->vx = RandomGen::get(-MAX_BALL_SPEED, MAX_BALL_SPEED);
        mParticles[i]->vy = RandomGen::get(-MAX_BALL_SPEED, MAX_BALL_SPEED);
        mParticles[i]->radius = RandomGen::get(MIN_BALL_RADIUS, MAX_BALL_RADIUS);
    }
}

ParticleSystem::~ParticleSystem()
{
    for (auto p : mParticles)
        delete p;
}

void ParticleSystem::render()
{
    clearScreen();
    glLoadIdentity();
    for (auto p : mParticles) {
        auto& c = COLOURS.at(BALL_COLOURS.at(p->is_colliding));
        glColor3f(c.r, c.g, c.b);
        drawCircle(p->x, p->y, p->radius);
    }
}

bool ParticleSystem::areTouching(Particle* a, Particle* b)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float hypot = std::hypot(dx, dy);
    float r = a->radius + b->radius;
    return hypot <= r;
}
void ParticleSystem::resetParticleCollisions()
{
    for (Particle* particle : mParticles) {
        particle->is_colliding = false;
    }
}

void ParticleSystem::resolveParticleCollision(Particle* particle1, Particle* particle2)
{
    // Vector between centers
    float dx = particle2->x - particle1->x;
    float dy = particle2->y - particle1->y;
    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance == 0.0f) {
        return; // Avoid division by zero
    }

    // Normal vector
    float nx = dx / distance;
    float ny = dy / distance;

    // Tangent vector
    float tx = -ny;
    float ty = nx;

    // Project velocities onto normal and tangent
    float v1n = particle1->vx * nx + particle1->vy * ny;
    float v1t = particle1->vx * tx + particle1->vy * ty;

    float v2n = particle2->vx * nx + particle2->vy * ny;
    float v2t = particle2->vx * tx + particle2->vy * ty;

    // Equal mass elastic collision → swap normal components
    // Equal mass inelastic collision using coefficient of restitution
    float v1n_after = (v1n + v2n - 1.0 * (v1n - v2n)) * 0.5f;
    float v2n_after = (v1n + v2n + 1.0 * (v1n - v2n)) * 0.5f;

    // Convert scalar normal/tangent velocities back to vectors
    particle1->vx = v1n_after * nx + v1t * tx;
    particle1->vy = v1n_after * ny + v1t * ty;

    particle2->vx = v2n_after * nx + v2t * tx;
    particle2->vy = v2n_after * ny + v2t * ty;

    // // Optional: positional correction to prevent sticking
    float overlap = (particle1->radius + particle2->radius) - distance;
    if (overlap > 0.0f) {
        float correction = overlap * 0.5f;
        particle1->x -= nx * correction;
        particle1->y -= ny * correction;
        particle2->x += nx * correction;
        particle2->y += ny * correction;
    }

    particle1->is_colliding = true;
    particle2->is_colliding = true;
}

void ParticleSystem::handleWallCollisions(Particle* particle)
{
    float abs_x_boundary = WORLD_MAX - particle->radius;
    float abs_y_boundary = WORLD_MAX - particle->radius;

    particle->x = std::clamp(particle->x, -abs_x_boundary, abs_x_boundary);
    particle->y = std::clamp(particle->y, -abs_y_boundary, abs_y_boundary);

    particle->vx = (std::abs(particle->x) == abs_x_boundary) ? -particle->vx : particle->vx;
    particle->vy = (std::abs(particle->y) == abs_y_boundary) ? -particle->vy : particle->vy;

    particle->is_colliding = (std::abs(particle->x) == abs_x_boundary || std::abs(particle->y) == abs_y_boundary) ? true : particle->is_colliding;
}

void ParticleSystem::moveParticle(Particle* particle, float timeDelta)
{
    particle->x += particle->vx * timeDelta;
    particle->y += particle->vy * timeDelta;
}

ParticleSystem_V1::ParticleSystem_V1()
    : ParticleSystem("Brute Force")
{
}

void ParticleSystem_V1::handleParticleCollisions()
{
    resetParticleCollisions();

    for (int i = 0; i < NUM_PARTICLES; i++) {
        for (int j = i; j < NUM_PARTICLES; j++) {
            if (areTouching(mParticles[i], mParticles[j])) {
                resolveParticleCollision(mParticles[i], mParticles[j]);
            }
        }
    }
}

void ParticleSystem_V1::update(float timeDelta)
{
    handleParticleCollisions();
    for (Particle* p : mParticles) {
        handleWallCollisions(p);
        moveParticle(p, timeDelta);
    }
}

int ParticleSystem_V2::cellX(float x)
{
    return int((x - WORLD_MIN) / CELL_SIZE);
}
int ParticleSystem_V2::cellY(float y)
{
    return int((y - WORLD_MIN) / CELL_SIZE);
}
int ParticleSystem_V2::cellIndex(int x, int y)
{
    return y * GRID_WIDTH + x;
}

void ParticleSystem_V2::rebuildGrid()
{
    // Remove all values in the grid
    for (auto& cell : grid) {
        cell.clear();
    }

    // Add each ball to the corresponding gridbox
    for (int i = 0; i < NUM_PARTICLES; i++) {
        Particle* p = mParticles[i];

        // Convert the x and y values to the grid x and y values
        int cx = std::clamp(cellX(p->x), 0, GRID_WIDTH - 1);
        int cy = std::clamp(cellY(p->y), 0, GRID_HEIGHT - 1);
        grid[cellIndex(cx, cy)].push_back(i);
    }
}
void ParticleSystem_V2::handleParticleCollisions()
{
    resetParticleCollisions();

    for (int i = 0; i < NUM_PARTICLES; i++) {
        // For every ball
        Particle* p = mParticles[i];

        int cx = std::clamp(cellX(p->x), 0, GRID_WIDTH - 1);
        int cy = std::clamp(cellY(p->y), 0, GRID_HEIGHT - 1);

        // Iterate over all the grid squares that are adjacent to the one the ball is in
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = cx + dx;
                int ny = cy + dy;

                // If the grid square is outside of bounds, continue
                if (nx < 0 || ny < 0 || nx >= GRID_WIDTH || ny >= GRID_HEIGHT) {
                    continue;
                }
                for (int j : grid[cellIndex(nx, ny)]) {
                    // Only check collisions with balls further in the grid
                    if (j <= i) {
                        continue;
                    }
                    // If they are touching,
                    if (areTouching(p, mParticles[j])) {
                        resolveParticleCollision(p, mParticles[j]);
                    }
                }
            }
        }
    }
}

ParticleSystem_V2::ParticleSystem_V2()
    : ParticleSystem("Spatial Partitioning")
{
    grid.resize(GRID_WIDTH * GRID_HEIGHT);
}
void ParticleSystem_V2::update(float timeDelta)
{
    rebuildGrid();
    handleParticleCollisions();

    for (Particle* p : mParticles) {
        handleWallCollisions(p);
        moveParticle(p, timeDelta);
    }
}
