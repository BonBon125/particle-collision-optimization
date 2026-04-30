#include "Recorder.h"
#include "Constants.h"
#include <GL/gl.h>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

ThreadedPNGRecorder::ThreadedPNGRecorder()
    : width(WIDTH_HEIGHT)
    , height(WIDTH_HEIGHT)
    , fps(60)
    , frameCounter(0)
{
}

ThreadedPNGRecorder::~ThreadedPNGRecorder()
{
    if (recording) {
        stop();
    }
}

void ThreadedPNGRecorder::start(int fps_)
{
    fps = fps_;
    frameCounter = 0;
    recording = true;
    done = false;

    writerThread = std::thread(&ThreadedPNGRecorder::threadedWriter, this);
    std::cout << "\n[Recorder] Threaded recording started." << std::endl;
}

void ThreadedPNGRecorder::captureFrame()
{
    if (!recording)
        return;

    std::vector<unsigned char> rgbBuffer(width * height * 3);
    std::vector<unsigned char> flippedBuffer(width * height * 3);

    // Grab the pixels from the GPU
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, rgbBuffer.data());

    // OpenGL is bottom-to-top, PNG is top-to-bottom. We must flip it.
    for (int y = 0; y < height; y++) {
        std::memcpy(&flippedBuffer[y * width * 3],
            &rgbBuffer[(height - 1 - y) * width * 3],
            width * 3);
    }

    // Push to the thread-safe queue
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        frameQueue.push(std::move(flippedBuffer));
    }
    queueCV.notify_one();
}

void ThreadedPNGRecorder::stop()
{
    if (!recording)
        return;

    recording = false;
    done = true;
    queueCV.notify_all();

    if (writerThread.joinable()) {
        writerThread.join();
    }
    std::cout << "[Recorder] Stopped. Total frames processed: " << frameCounter << std::endl;
}

void ThreadedPNGRecorder::threadedWriter()
{
    while (!done || !frameQueue.empty()) {
        std::vector<unsigned char> frame;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCV.wait(lock, [this] { return !frameQueue.empty() || done; });

            if (!frameQueue.empty()) {
                frame = std::move(frameQueue.front());
                frameQueue.pop();
            } else {
                continue;
            }
        }

        // Build filename: ../images/frame_00001.png
        std::ostringstream name;
        name << "../images/frame_" << std::setw(5) << std::setfill('0') << frameCounter++ << ".png";

        stbi_write_png(name.str().c_str(), width, height, 3, frame.data(), width * 3);
    }
}

void ThreadedPNGRecorder::exportMP4(const std::string& outputFile)
{
    stop();
    std::ostringstream cmd;
    cmd << "ffmpeg -y -framerate " << fps
        << " -i ../images/frame_%05d.png -pix_fmt yuv420p "
        << outputFile;

    std::cout << "[Recorder] Exporting MP4 via FFmpeg..." << std::endl;
    if (std::system(cmd.str().c_str()) == 0) {
        std::cout << "[Recorder] Export complete: " << outputFile << std::endl;
    }
}

void ThreadedPNGRecorder::exportGIF(const std::string& outputFile)
{
    stop();

    // Step 1: Generate a high-quality palette
    std::ostringstream cmd1;
    cmd1 << "ffmpeg -y -framerate " << fps
         << " -i ../images/frame_%05d.png "
         << "-vf \"fps=" << fps << ",scale=480:-1:flags=lanczos,palettegen\" gif-directory/palette.png";

    std::system(cmd1.str().c_str());

    // Step 2: Use palette to generate the GIF
    std::ostringstream cmd2;
    cmd2 << "ffmpeg -y -framerate " << fps
         << " -i ../images/frame_%05d.png -i gif-directory/palette.png "
         << "-lavfi \"fps=" << fps << ",scale=480:-1:flags=lanczos[x];[x][1:v]paletteuse\" "
         << outputFile;

    std::cout << "[Recorder] Exporting GIF..." << std::endl;
    std::system(cmd2.str().c_str());
}
