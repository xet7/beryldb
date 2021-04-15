
#include <signal.h>

#include "beryldb.h"
#include "exit.h"
#include "printb.h"

std::unique_ptr<BerylDB> Kernel = nullptr;

/* BerylDB's main. */

BerylStart
{
        Kernel = std::make_unique<BerylDB>(argc, argv);
        return 0;
}

BerylDB::BerylDB(int argc, char** argv) : ConfigFile(DEFAULT_CONFIG)
{
        /* Main link. */

        Kernel = std::unique_ptr<BerylDB>(this);

        /* Refreshing time for the very first time. */

        this->Refresh();

}