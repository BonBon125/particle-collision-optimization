#pragma once
#include "ParticleSystem.h"
#include <GLFW/glfw3.h>
class Logging {
private:
    bool mEnableLogging;

public:
    Logging(GLFWwindow* window, bool enableLogging);

    void logFrameData(float time, ParticleSystem& particleSystem);

    void displayLog();

    ~Logging();
};
