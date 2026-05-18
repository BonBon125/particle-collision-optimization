#include <GLFW/glfw3.h>
#include <memory>
#include <string>

#include "Constants.h"
#include "Logging.h"
#include "ParticleSystem.h"
#include "ParticleSystemApp.h"
#include "RecordingTool.h"
#include "imgui.h"

bool ParticleSystemApp::init(CommandLineArguments& commandLineArguments)
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

    ParticleSystemApp::configureAppSettings(commandLineArguments);

    mRecordingTool.setOptions(mAppSettings.recordingMethod, mAppSettings.recordingDuration);
    mLogging = std::make_unique<Logging>(window, mAppSettings.enableLogging);

    // We want to select ParticleSystem based on options

    if (mAppSettings.particleSystemVersion == 1) {
        mParticleSystem = std::make_unique<ParticleSystem_V1>();
    } else if (mAppSettings.particleSystemVersion == 2) {
        mParticleSystem = std::make_unique<ParticleSystem_V2>();
    } else {
        mParticleSystem = std::make_unique<ParticleSystem_V2>();
    }
    mRecordingTool.init();

    mBeginningTime = mLastTime = (float)glfwGetTime();

    return true;
}

void ParticleSystemApp::configureAppSettings(CommandLineArguments& commandLineArguments)
{
    mAppSettings.recordingMethod = commandLineArguments.getArguments().at("--recording-method").value;
    std::string recordingDurationString = commandLineArguments.getArguments().at("--recording-duration").value;
    if (recordingDurationString != "") {
        mAppSettings.recordingDuration = std::stoi(recordingDurationString);
    }
    std::string particleSystemVersion = commandLineArguments.getArguments().at("--particle-system-version").value;
    if (particleSystemVersion != "") {
        mAppSettings.particleSystemVersion = std::stoi(particleSystemVersion);
    }
    mAppSettings.enableLogging = commandLineArguments.getArguments().at("-logging").value != "";
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

    if (mRecordingTool.isRecording() && totalTime > mRecordingTool.getRecordingDuration()) {
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

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.4f)); // Set window background to transparent
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.0f, 0.4f)); // Set window background to transparent
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Set window background to red
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Set window background to red
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 1.0f, 0.0f, 0.2f)); // Set window background to red
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.0f, 1.0f, 0.0f, 0.3f)); // Set window background to red
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.4f)); // Set window background to red
    ImGui::Begin("Debug");
    ImGui::SliderInt("Set Num Particles", &numParticles, 0, NUM_PARTICLES);
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::End();

    mParticleSystem->updateNumParticles(numParticles);
    return;
}

void ParticleSystemApp::render()
{
    mParticleSystem->render();
    mLogging->displayLog();
}
