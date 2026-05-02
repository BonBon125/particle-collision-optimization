#include "RecordingTool.h"
#include "Utils.h"
#include <iostream>

RecordingTool::RecordingTool()
{
    std::string message = "Do you want to (y)Record: ";

    record_option = getInput(message);

    if (record_option == 'y') {
        recorder.start(60);
    }
}

void RecordingTool::handleFrame()
{
    if (recorder.recording) {
        recorder.captureFrame();
    }
}

bool RecordingTool::isRecording()
{
    return recorder.recording;
}

char RecordingTool::getInput(std::string prompt)
{
    print(prompt);

    // Visual indicator for the input field
    std::cout << " > ";

    char input;
    std::cin >> input;

    // Optional: Clear the buffer in case they typed more than one char
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return input;
}

RecordingTool::~RecordingTool()
{
    if (record_option == 'y') {
        std::string message = "Do you want to export as (1)MP4 or (2)GIF: ";
        char answer = getInput(message);

        if (answer == '1') {
            recorder.exportMP4();
        } else {
            recorder.exportGIF();
        }
    }
}
