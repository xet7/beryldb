
#include "beryldb.h"
#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "managers/hash.h"
#include "managers/lists.h"

CoreManager::CoreManager()
{

}

void CoreManager::Open()
{
       std::shared_ptr<CoreDatabase> New;
       New = std::make_shared<CoreDatabase>();
       New->created = Kernel->Now();
       bprint(DONE, "Core database opened.");
       New->Open();
       this->DB = New;
}

