#include "RecordingTool.h"
#include "Utils.h"
#include <iostream>
#include <string>

void RecordingTool::setOptions(std::string recordingMethod, int recordingDuration)
{
    mRecordingMethod = recordingMethod;
    mRecordingDuration = recordingDuration;
    mRecordingOption = recordingMethod != "" ? 'y' : 'n';
}

void RecordingTool::init()
{
    if (mRecordingOption == 'y') {
        recorder.start();
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

int RecordingTool::getRecordingDuration()
{
    return mRecordingDuration;
}

RecordingTool::~RecordingTool()
{
    if (mRecordingOption == 'y') {
        if (mRecordingMethod == "MP4") {
            recorder.exportMP4();
        } else if (mRecordingMethod == "GIF") {
            recorder.exportGIF();
        }
    }
}
