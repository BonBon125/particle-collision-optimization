#pragma once
#include <GLFW/glfw3.h>
#include <memory>

#include "Logging.h"
#include "ParticleSystem.h"
#include "RecordingTool.h"

class ParticleSystemApp {
public:
    bool init();
    void run();
    ~ParticleSystemApp();

private:
    GLFWwindow* window = nullptr;
    std::unique_ptr<ParticleSystem> mParticleSystem;
    std::unique_ptr<Logging> mLogging;
    RecordingTool mRecordingTool;

    float mBeginningTime = 0.0f;
    float mLastTime = 0.0f;

    void setupOpenGL();
    void updateTime();
    void handleEvents();
    void render();
    void update();
};
