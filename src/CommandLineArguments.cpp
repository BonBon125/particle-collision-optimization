#include "CommandLineArguments.h"
#include <iostream>

bool CommandLineArguments::parse(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i) {
        std::string key = argv[i];

        if (mArguments.find(key) == mArguments.end()) {
            std::cerr << "Unknown argument: " << key << "\n";
            return false;
        }
        // If the argument requires a value (e.g., --duration 10)
        if (i + 1 < argc && argv[i + 1][0] != '-') {
            mArguments[key].value = argv[++i];
        } else {
            mArguments[key].value = "true";
        }
    }
    return true;
}

void CommandLineArguments::dump()
{
    for (auto const& [key, config] : mArguments) {
        std::cout << config.category << " | " << key << ": " << config.value << "\n";
    }
}
const std::unordered_map<std::string, CommandLineArguments::ArgConfig>& CommandLineArguments::getArguments()
{
    return mArguments;
}
