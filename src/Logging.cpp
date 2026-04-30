#include "Logging.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Logging::Logging(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130"); // Use 130 for legacy compatibility

    // Setup style
    ImGui::StyleColorsDark();
}

void Logging::logFrameData(float time, ParticleSystem& particleSystem)
{
    // 1. Start ImGui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 2. Define your Text Overlay
    // We use a window with specific flags to make it look like an overlay
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.4f));
    ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs);
    ImGui::SetWindowFontScale(1.5f); // Make text 50% larger in this window only

    // ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Collision Handling Method: %s", particleSystem.getMethodName().c_str());
    // ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Simulation Data:");

    ImGui::Text("Collision Handling Method: %s", particleSystem.getMethodName().c_str());
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Time: %.2f s", time);
    ImGui::Text("Particle Count: %i", NUM_PARTICLES);

    ImGui::End();
    ImGui::PopStyleColor();
}

void Logging::displayLog()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Logging::~Logging()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
