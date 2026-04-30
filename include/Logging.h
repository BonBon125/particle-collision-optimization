#pragma once
#include "ParticleSystem.h"
#include <GLFW/glfw3.h>
class Logging {
public:
    Logging(GLFWwindow* window);

    void logFrameData(float time, ParticleSystem& particleSystem);

    void displayLog();

    ~Logging();
};
