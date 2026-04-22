#include <GLFW/glfw3.h>
#include <iostream>

#include "Constants.h"
#include "ParticleSystem.h"
#include "Recorder.h"
#include "Utils.h"

int main()
{
    if (!glfwInit())
        return -1;

    ThreadedPNGRecorder recorder;
    // We want to check if we want to record
    std::cout << "\nDo you want to (y)Record" << std::endl;

    char record_option;
    std::cin >> record_option;

    if (record_option == 'y') {
        recorder.start(60);
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH_HEIGHT, WIDTH_HEIGHT, "Bouncing Ball", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

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

        particleSystem.update(deltaTime);
        particleSystem.render();

        displayFPS(deltaTime);

        // Capture frame every render
        if (recorder.recording) {
            recorder.captureFrame();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (recorder.recording && currentTime - beginning_time > RECORDING_DURATION) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    glfwTerminate();

    if (record_option == 'y') {
        std::cout << "\nDo you want to export as (1)MP4 or (2)GIF" << std::endl;

        char answer;
        std::cin >> answer;

        if (answer == '1') {
            recorder.exportMP4();
        } else {
            recorder.exportGIF();
        }
    }

    return 0;
}
