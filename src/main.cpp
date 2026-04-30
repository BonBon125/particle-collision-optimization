#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "Constants.h"
#include "ParticleSystem.h"
#include "RecordingTool.h"

int main()
{
    if (!glfwInit())
        return -1;

    // ThreadedPNGRecorder recorder;
    // TODO: We want to make a class that when constructed, takes the recorder and sets the record option
    RecordingTool cli_tool;

    GLFWwindow* window = glfwCreateWindow(WIDTH_HEIGHT, WIDTH_HEIGHT, "Bouncing Ball", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130"); // Use 130 for legacy compatibility

    // Setup style
    ImGui::StyleColorsDark();

    // 2D projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glfwSwapBuffers(window);
    glfwPollEvents();

    ParticleSystem_V2 particleSystem = ParticleSystem_V2();

    float beginning_time = (float)glfwGetTime();
    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // 1. Start ImGui Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 2. Define your Text Overlay
        // We use a window with specific flags to make it look like an overlay
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
        ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs);
        ImGui::SetWindowFontScale(1.5f); // Make text 50% larger in this window only

        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Collision Handling Method: %s", particleSystem.getMethodName().c_str());
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Simulation Data:");
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::Text("Time: %.2f s", currentTime - beginning_time);
        ImGui::Text("Particle Count: %i", NUM_PARTICLES);

        ImGui::End();

        particleSystem.update(deltaTime);
        particleSystem.render();

        // 4. Render ImGui onto the screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        cli_tool.handleFrame(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (cli_tool.isRecording() && currentTime - beginning_time > RECORDING_DURATION) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}
