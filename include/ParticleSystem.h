#pragma once
#include "Particle.h"
#include <string>
#include <vector>

class ParticleSystem {
public:
    ParticleSystem(std::string methodName);
    virtual ~ParticleSystem();
    void render();
    int getNumCollidingParticles();
    int getNumParticles();
    void updateNumParticles(int newNumParticles);
    virtual void update(float timeDelta) = 0;
    std::string getMethodName() const { return mCollisionHandlingMethod; }

protected:
    std::string mCollisionHandlingMethod;
    std::vector<Particle*> mParticles;
    int numCollidingParticles = 0;
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
    ParticleSystem_V1();
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
