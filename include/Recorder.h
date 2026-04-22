#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

class ThreadedPNGRecorder {
public:
    ThreadedPNGRecorder();
    ~ThreadedPNGRecorder();

    void start(int fps = 60);
    void captureFrame();
    void stop();

    void exportMP4(const std::string& outputFile = "../gif-directory/simulation.mp4");
    void exportGIF(const std::string& outputFile = "../gif-directory/simulation.gif");

    std::atomic<bool> recording { false };

private:
    void threadedWriter();

    int width;
    int height;
    int fps;
    int frameCounter;
    std::atomic<bool> done { false };

    std::queue<std::vector<unsigned char>> frameQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::thread writerThread;
};
