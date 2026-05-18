#pragma once
#include <GLFW/glfw3.h>
#include <memory>

#include "CommandLineArguments.h"
#include "Logging.h"
#include "ParticleSystem.h"
#include "RecordingTool.h"

class ParticleSystemApp {
public:
    bool init(CommandLineArguments& commandLineArguments);
    void run();
    ~ParticleSystemApp();

private:
    struct AppSettings {
        std::string recordingMethod = "none";
        int recordingDuration = 4;
        int particleSystemVersion = 2;
        bool enableLogging = false;
    } mAppSettings;

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
    void configureAppSettings(CommandLineArguments& commandLineArguments);
};
