#include "ParticleSystemApp.h"

int main()
{
    ParticleSystemApp app;
    if (!app.init())
        return 1;
    app.run();
    return 0;
}
