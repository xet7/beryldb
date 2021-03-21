
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
