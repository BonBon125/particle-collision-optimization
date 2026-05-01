#pragma once
#include "Recorder.h"
#include "string"

class RecordingTool {
private:
    ThreadedPNGRecorder recorder;
    char record_option;

public:
    RecordingTool();
    void handleFrame(float dt);
    bool isRecording();
    char getInput(std::string prompt);
    ~RecordingTool();
};
