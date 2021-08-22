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
#include "managers/keys.h"
#include "managers/globals.h"

std::mutex FutureManager::mute;

FutureManager::FutureManager()
{

}

signed int FutureManager::Add(std::shared_ptr<Database> database, signed int schedule, const std::string& key, const std::string& value, unsigned int select, bool epoch)
{
        if (schedule < 0)
        {
                return -1;
        }
        
        if (FutureManager::GetTIME(database, key, select) > 0)
        {
              FutureManager::Delete(database, key, select);
        }

        std::lock_guard<std::mutex> lg(FutureManager::mute);
        
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
        New.added = Now;
        New.secs = schedule;
        New.select = select;
        Kernel->Store->Futures->FutureList.insert(std::make_pair(New.schedule, New));
        return New.schedule;
}

bool FutureManager::Delete(std::shared_ptr<Database> database, const std::string& key, unsigned int select)
{
        FutureMap& all = Kernel->Store->Futures->GetFutures();

        /* Nothing to delete, at all. */

        if (!all.size())
        {
                return false;
        }

        std::lock_guard<std::mutex> lg(FutureManager::mute);

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
              
              GlobalHelper::FutureExecute(entry.database, entry.select, entry.key);
              it++;
        }
}

FutureEntry FutureManager::Find(std::shared_ptr<Database> database, const std::string& key, unsigned int select)
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

std::tuple<int, std::string> FutureManager::GetVal(std::shared_ptr<Database> database, const std::string& key, unsigned int select)
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

bool FutureManager::Exec(std::shared_ptr<Database> database, const std::string& key, unsigned int select)
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

        FutureManager::Delete(database, key, select);
        return true;
}

signed int FutureManager::GetTIME(std::shared_ptr<Database> database, const std::string& key, unsigned int select)
{
        FutureMap& futures = Kernel->Store->Futures->GetFutures();
        
        std::lock_guard<std::mutex> lg(FutureManager::mute);

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

signed int FutureManager::GetTTE(std::shared_ptr<Database> database, const std::string& key, unsigned int select)
{
      std::lock_guard<std::mutex> lg(FutureManager::mute);
      return FutureManager::GetTIME(database, key, select);
}

FutureMap& FutureManager::GetFutures()
{
       std::lock_guard<std::mutex> lg(FutureManager::mute);
       return this->FutureList;
}

void FutureManager::Reset()
{
      FutureMap& expiring = Kernel->Store->Futures->GetFutures();

      if (!expiring.size())
      {
              return;
      }

      std::lock_guard<std::mutex> lg(FutureManager::mute);

      for (FutureMap::iterator it = expiring.begin(); it != expiring.end(); it++)
      {
              FutureEntry entry = it->second;
              GlobalHelper::FutureGlobalCancel(entry.database, entry.select, entry.key);
      }  
}

unsigned int FutureManager::DatabaseReset(const std::string& dbname)
{
      FutureMap& expiring = Kernel->Store->Futures->GetFutures();

      if (!expiring.size())
      {
              return 0;
      }

      unsigned int counter = 0;

      std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);

      if (!database)
      {
           return 0;
      }

      std::lock_guard<std::mutex> lg(FutureManager::mute);
      
      for (FutureMap::iterator it = expiring.begin(); it != expiring.end(); )
      {
            if (it->second.database == database)
            {
                       FutureEntry entry = it->second;
                       GlobalHelper::FutureGlobalCancel(entry.database, entry.select, entry.key);
                       counter++;
            }

            it++;
      }  
      
      return counter;
}

unsigned int FutureManager::SelectReset(const std::string& dbname, unsigned int select)
{
      /* Keeps track of deleted expires */

      unsigned int counter = 0;

      FutureMap& futures = Kernel->Store->Futures->GetFutures();

      std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);

      if (!database)
      {
            return 0;
      }

      std::lock_guard<std::mutex> lg(FutureManager::mute);

      for (FutureMap::iterator it = futures.begin(); it != futures.end(); )
      {
            if (it->second.select == select && it->second.database == database)
            {
                    FutureEntry entry = it->second;
                    GlobalHelper::FutureGlobalCancel(entry.database, entry.select, entry.key);
                    counter++;
            }

            it++;
      }

      return counter;
}

void FutureManager::PreDBClose(const std::string& dbname)
{
      FutureMap& futures = Kernel->Store->Futures->GetFutures();

      if (!futures.size())
      {
              return;
      }

      std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);

      if (!database)
      {
           return;
      }

      std::lock_guard<std::mutex> lg(FutureManager::mute);

      for (FutureMap::iterator it = futures.begin(); it != futures.end(); )
      {
                if (it->second.database != database)
                {
                       it++;
                       continue;
                }

                Kernel->Store->Futures->FutureList.erase(it++);
      }
}

unsigned int FutureManager::Count(std::shared_ptr<Database> database, unsigned int select)
{
        FutureMap& expires = Kernel->Store->Futures->GetFutures();

        unsigned int counter = 0;

        for (FutureMap::iterator it = expires.begin(); it != expires.end(); it++)
        {
                FutureEntry entry = it->second;

                if (entry.select == select && entry.database == database)
                {
                       counter++;
                }
        }

        return counter;
}

void FutureManager::DatabaseDestroy(const std::string& dbname)
{
      FutureMap& futures = Kernel->Store->Futures->GetFutures();

      if (!futures.size())
      {
              return;
      }

      std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);

      if (!database)
      {
           return;
      }

      std::lock_guard<std::mutex> lg(FutureManager::mute);

      for (FutureMap::iterator it = futures.begin(); it != futures.end(); )
      {
            if (it->second.database == database)
            {
                    futures.erase(it++);
                    continue;
            }

            it++;
      }  
}
