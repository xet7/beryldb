/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 - Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"
#include "algo.h"
#include "brldb/futures.h"
#include "brldb/dbflush.h"
#include "brldb/database.h"
#include "brldb/datathread.h"
#include "brldb/dbnumeric.h"
#include "brldb/dbmanager.h"
#include "managers/keys.h"

std::mutex FutureManager::mute;

FutureManager::FutureManager()
{

}

signed int FutureManager::Add(signed int schedule, const std::string& key, const std::string& value, const std::string& select, bool epoch)
{
        if (schedule < 0)
        {
                return -1;
        }
        
        if (epoch)
        {
              /* We cannot add an expiring epoch that has already passed. */
              
              if (schedule < Kernel->Now())
              {
                    return -1;
              }
        }

        if (FutureManager::TriggerTIME(key, select) > 0)
        {
              FutureManager::Delete(key, select);
        }
        
        FutureManager::mute.lock();

        time_t Now = Kernel->Now();
        FutureEntry New;

        New.epoch = epoch;

        if (epoch)
        {
            New.schedule = schedule;
        }
        else
        {
            New.schedule = Now + schedule;
        }
        
        New.key = key;
        New.value = value;
        New.added = Now;
        New.secs = schedule;
        New.select = select;
        
        Kernel->Store->Futures->FutureList.insert(std::make_pair(New.schedule, New));

        FutureManager::mute.unlock();

        return New.schedule;
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
              KeyHelper::Set(Kernel->Clients->Global, Kernel->Store->Default, entry.select, entry.key, entry.value, "", TYPE_NONE, true);  
              Kernel->Store->Futures->FutureList.erase(it++);
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


