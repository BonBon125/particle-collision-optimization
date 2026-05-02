#include <GLFW/glfw3.h>
#include <memory>

#include "Constants.h"
#include "Logging.h"
#include "ParticleSystem.h"
#include "ParticleSystemApp.h"
#include "RecordingTool.h"
#include "imgui.h"

bool ParticleSystemApp::init()
{
    if (!glfwInit())
        return false;

    window = glfwCreateWindow(WIDTH_HEIGHT, WIDTH_HEIGHT, "Bouncing Ball", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    setupOpenGL();
    mLogging = std::make_unique<Logging>(window);
    mParticleSystem = std::make_unique<ParticleSystem_V2>();

    mBeginningTime = mLastTime = (float)glfwGetTime();

    return true;
}

void ParticleSystemApp::run()
{
    while (!glfwWindowShouldClose(window)) {
        updateTime();
        handleEvents();
        update();
        render();
    }
}

ParticleSystemApp::~ParticleSystemApp()
{
    glfwTerminate();
}

void ParticleSystemApp::setupOpenGL()
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}
void ParticleSystemApp::updateTime()
{
    float currentTime = (float)glfwGetTime();
    float deltaTime = currentTime - mLastTime;
    float totalTime = currentTime - mBeginningTime;
    mLastTime = currentTime;

    mParticleSystem->update(deltaTime);
    mLogging->logFrameData(totalTime, *mParticleSystem);
    mRecordingTool.handleFrame();

    if (mRecordingTool.isRecording() && totalTime > RECORDING_DURATION) {
        glfwSetWindowShouldClose(window, true);
    }
}
void ParticleSystemApp::handleEvents()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void ParticleSystemApp::update()
{
    int numParticles = mParticleSystem->getNumParticles();
    ImGui::SliderInt("Set Num Particles", &numParticles, 0, NUM_PARTICLES);
    mParticleSystem->updateNumParticles(numParticles);
    return;
}

void ParticleSystemApp::render()
{
    mParticleSystem->render();
    mLogging->displayLog();
}
