#include "RecordingTool.h"
#include <iostream>

RecordingTool::RecordingTool()
{
    std::cout << "Do you want to (y)Record: ";
    std::cin >> record_option;
    std::cout << std::endl;

    if (record_option == 'y') {
        recorder.start(60);
    }
}

void RecordingTool::handleFrame(float dt)
{
    if (recorder.recording) {
        recorder.captureFrame();
    }
}

bool RecordingTool::isRecording()
{
    return recorder.recording;
}

RecordingTool::~RecordingTool()
{
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
}
