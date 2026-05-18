#include "CommandLineArguments.h"
#include "ParticleSystemApp.h"
#include <cstring>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    // TODO: Parse the following optional arguments
    CommandLineArguments commandLineArguments;
    if (!commandLineArguments.parse(argc, argv))
        return 1;

    ParticleSystemApp app;
    if (!app.init(commandLineArguments))
        return 1;
    app.run();
    return 0;
}
