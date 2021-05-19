/*
 * BerylDB - A lightweight database.
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

FutureManager::FutureManager()
{

}

signed int FutureManager::Add(std::shared_ptr<Database> database, signed int schedule, const std::string& key, const std::string& value, const std::string& select, bool epoch)
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

        if (FutureManager::TriggerTIME(database, key, select) > 0)
        {
              FutureManager::Delete(database, key, select);
        }
        
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
        
        New.database = database;
        New.key = key;
        New.value = value;
        New.added = Now;
        New.secs = schedule;
        New.select = select;
        
        Kernel->Store->Futures->FutureList.insert(std::make_pair(New.schedule, New));

        return New.schedule;
}

bool FutureManager::Delete(std::shared_ptr<Database> database, const std::string& key, const std::string& select)
{
        FutureMap& all = Kernel->Store->Futures->GetFutures();

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
                
                if (it->second.database != database)
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
        
        for (FutureMap::iterator it = futures.begin(); it != futures.end(); )
        {
              if (it->first >= TIME)
              {
                      break;
              }
              
              FutureEntry entry = it->second;
              KeyHelper::Set(Kernel->Clients->Global, entry.database, entry.select, entry.key, entry.value, "", TYPE_NONE, true);  
              Kernel->Store->Futures->FutureList.erase(it++);
        }
}

FutureEntry FutureManager::Find(std::shared_ptr<Database> database, const std::string& key, const std::string& select)
{
        FutureMap& current = Kernel->Store->Futures->GetFutures();

        for (FutureMap::iterator it = current.begin(); it != current.end(); it++)
        {
                  if (it->second.key == key && it->second.select == select && it->second.database == database)
                  {
                          return it->second;
                  }
        }

        /* Not found, not expiring. */

        throw KernelException("ne");
}

std::tuple<int, std::string> FutureManager::GetVal(std::shared_ptr<Database> database, const std::string& key, const std::string& select)
{
        FutureEntry entry;
 
        try
        {
              entry = FutureManager::Find(database, key, select);
        }
        catch (KernelException& err)
        {
              return std::make_tuple(0, "");
        }
        
        return std::make_tuple(1, entry.value);
}

bool FutureManager::Exec(std::shared_ptr<Database> database, const std::string& key, const std::string& select)
{
        FutureEntry entry;
        
        try
        {
              entry = FutureManager::Find(database, key, select);
        }
        catch (KernelException& err)
        {
              return false;
        }

        KeyHelper::Set(Kernel->Clients->Global, entry.database, entry.select, entry.key, entry.value, "", TYPE_NONE, true);  
        FutureManager::Delete(database, key, select);
        return true;
}

signed int FutureManager::TriggerTIME(std::shared_ptr<Database> database, const std::string& key, const std::string& select)
{
        FutureMap& futures = Kernel->Store->Futures->GetFutures();

        for (FutureMap::iterator it = futures.begin(); it != futures.end(); it++)
        {
                        if (it->second.key == key && it->second.select == select && it->second.database == database)
                        {
                                return it->first;
                        }
        }

        /* Not found, not expiring. */

        return -1;
}


signed int FutureManager::GetTTE(std::shared_ptr<Database> database, const std::string& key, const std::string& select)
{
      return FutureManager::TriggerTIME(database, key, select);
}

FutureMap& FutureManager::GetFutures()
{
       return this->FutureList;
}

void FutureManager::Reset()
{
      Kernel->Store->Futures->FutureList.clear();
}

unsigned int FutureManager::SReset(std::shared_ptr<Database> database, const std::string& select)
{
      /* Keeps track of deleted expires */

      unsigned int counter = 0;

      FutureMap& expiring = Kernel->Store->Futures->GetFutures();

      for (FutureMap::iterator it = expiring.begin(); it != expiring.end(); )
      {
            if (it->second.select == select && it->second.database == database)
            {
                       Kernel->Store->Futures->FutureList.erase(it++);
                       counter++;
            }
            else
            {
                       it++;
            }
      }

      return counter;
}

