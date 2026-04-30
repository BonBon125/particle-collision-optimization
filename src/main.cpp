#include <GLFW/glfw3.h>

#include "Constants.h"
#include "Logging.h"
#include "ParticleSystem.h"
#include "RecordingTool.h"

int main()
{
    if (!glfwInit())
        return -1;

    RecordingTool recordingTool;

    GLFWwindow* window = glfwCreateWindow(WIDTH_HEIGHT, WIDTH_HEIGHT, "Bouncing Ball", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    Logging logging = Logging(window);

    // 2D projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glfwSwapBuffers(window);
    glfwPollEvents();

    ParticleSystem_V2 particleSystem = ParticleSystem_V2();

    float beginningTime = (float)glfwGetTime();
    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        float totalTime = currentTime - beginningTime;
        lastTime = currentTime;

        logging.logFrameData(totalTime, particleSystem);

        particleSystem.update(deltaTime);
        particleSystem.render();

        logging.displayLog();

        recordingTool.handleFrame(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (recordingTool.isRecording() && totalTime > RECORDING_DURATION) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    glfwTerminate();

    return 0;
}
