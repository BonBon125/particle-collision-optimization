#pragma once
#include <cstring>
#include <string>
#include <sys/types.h>
#include <unordered_map>

class CommandLineArguments {
private:
    struct ArgConfig {
        std::string category;
        std::string value;
    };

    std::unordered_map<std::string, ArgConfig> mArguments = {
        // { "--recording", { "recorder", "" } },
        { "--recording-method", { "recorder", "" } },
        { "--recording-duration", { "recorder", "" } },
        { "-logging", { "logging", "" } },
        { "--particle-system-version", { "particle-system", "" } }
    };

public:
    bool parse(int argc, char* argv[]);
    void dump();
    const std::unordered_map<std::string, ArgConfig>& getArguments();
};
