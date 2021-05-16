

#include "beryl.h"
#include "algo.h"
#include "brldb/dbflush.h"
#include "brldb/database.h"
#include "brldb/datathread.h"
#include "brldb/dbnumeric.h"
#include "brldb/dbmanager.h"
#include "managers/keys.h"
#include "managers/maps.h"
#include "managers/settings.h"

std::mutex FutureManager::mute;

FutureManager::FutureManager()
{

}

bool FutureManager::Delete(const std::string& key, const std::string& select)
{
        FutureMap& all = Kernel->Store->Futures->GetFutures();

        std::lock_guard<std::mutex> lg(FutureManager::mute);

        /* Nothing to delete, at all. */

        if (!all.size())
        {
                return false;
        }

        for (FutureMap::iterator it = all.begin(); it != all.end(); )
        {
                if (it->second.key != key)
                {
                    it++;
                    continue;
                }

                if (it->second.select != select)
                {
                     it++;
                     continue;
                }

                Kernel->Store->Futures->FutureList.erase(it++);
                return true;
        }

        /* Key not found. */

        return false;
}

void FutureManager::Flush(time_t TIME)
{
        FutureMap& futures = Kernel->Store->Futures->GetFutures();

        if (!futures.size())
        {
              return;
        }
        
        FutureManager::mute.lock();
        
        for (FutureMap::iterator it = futures.begin(); it != futures.end(); )
        {
              if (it->first >= TIME)
              {
                      break;
              }
              
              FutureEntry entry = it->second;
          
        }
        
        FutureManager::mute.unlock();    
}

signed int FutureManager::TriggerTIME(const std::string& key, const std::string& select)
{
        FutureMap& futures = Kernel->Store->Futures->GetFutures();

        std::lock_guard<std::mutex> lg(FutureManager::mute);

        for (FutureMap::iterator it = futures.begin(); it != futures.end(); it++)
        {
                        if (it->second.key == key && it->second.select == select)
                        {
                                return it->first;
                        }
        }

        /* Not found, not expiring. */

        return -1;
}


/*signed int FutureManager::GetTTE(const std::string& key, const std::string& select)
{

}*/

FutureMap& FutureManager::GetFutures()
{
       std::lock_guard<std::mutex> lg(FutureManager::mute);
       return this->FutureList;
}

void FutureManager::Reset()
{
      std::lock_guard<std::mutex> lg(FutureManager::mute);
      Kernel->Store->Futures->FutureList.clear();
}


