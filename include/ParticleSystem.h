#pragma once
#include "Constants.h"
#include "Particle.h"
#include <array>
#include <vector>

class ParticleSystem {
public:
    ParticleSystem();
    virtual ~ParticleSystem();
    void render();
    virtual void update(float timeDelta) = 0;

protected:
    std::array<Particle*, NUM_PARTICLES> mParticles;
    void resetParticleCollisions();
    void resolveParticleCollision(Particle* p1, Particle* p2);
    void handleWallCollisions(Particle* p);
    void moveParticle(Particle* p, float dt);
    bool areTouching(Particle* a, Particle* b);
};

class ParticleSystem_V1 : public ParticleSystem {
private:
    void handleParticleCollisions();

public:
    void update(float timeDelta) override;
};

class ParticleSystem_V2 : public ParticleSystem {
private:
    std::vector<std::vector<int>> grid;
    int cellX(float x);
    int cellY(float y);
    int cellIndex(int x, int y);
    void rebuildGrid();
    void handleParticleCollisions();

public:
    ParticleSystem_V2();
    void update(float timeDelta) override;
};
