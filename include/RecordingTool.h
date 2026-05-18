#pragma once
#include "Recorder.h"
#include "string"

class RecordingTool {
private:
    ThreadedPNGRecorder recorder;
    std::string mRecordingMethod = "";
    int mRecordingDuration;
    char mRecordingOption;

public:
    void init();
    void setOptions(std::string recordingMethod = "", int recordingDuration = 60);
    void handleFrame();
    bool isRecording();
    char getInput(std::string prompt);
    int getRecordingDuration();
    ~RecordingTool();
};
