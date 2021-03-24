
#include <signal.h>

#include "beryl.h"

Beryl* Nucleus = NULL;

BerylStart
{
        new Beryl(argc, argv);
        Nucleus->Initialize();
        delete Nucleus;
        return 0;
}

Beryl::Beryl(int argc, char** argv) : ConfigFile(DEFAULT_CONFIG)
{
        /* Main link. */

        Nucleus = this;

        /* Refreshing time for the very first time. */

        this->TimeRefresh();
}

void Beryl::Initialize()
{
        this->TimeRefresh();
        time_t PREV_TIME = this->TIME.tv_sec;

        /* We wait 2 seconds before start processing requests. */

        usleep(200000);

        do
        {
        
        }
        while (true);
}