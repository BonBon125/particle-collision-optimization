#pragma once
#include "Recorder.h"
class RecordingTool {
private:
    ThreadedPNGRecorder recorder;
    char record_option;

public:
    RecordingTool();
    void handleFrame(float dt);
    bool isRecording();
    ~RecordingTool();
};
