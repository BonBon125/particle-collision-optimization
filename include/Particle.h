#pragma once

struct Particle {
    float x, y;
    float vx, vy;
    float radius;
    bool is_colliding = false;
};
