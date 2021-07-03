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
#include "brldb/dbflush.h"
#include "brldb/database.h"
#include "brldb/datathread.h"
#include "brldb/dbnumeric.h"
#include "brldb/dbmanager.h"
#include "managers/keys.h"
#include "managers/maps.h"
#include "managers/settings.h"
#include "managers/expires.h"

std::mutex ExpireManager::mute;

ExpireManager::ExpireManager()
{

}

bool ExpireManager::Delete(std::shared_ptr<Database> database, const std::string& key, const std::string& select)
{
        ExpireMap& all = Kernel->Store->Expires->GetExpires();

        std::lock_guard<std::mutex> lg(ExpireManager::mute);

        /* Nothing to delete, at all. */
        
        if (!all.size())
        {
                return false;
        }

        for (ExpireMap::iterator it = all.begin(); it != all.end(); )
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

                Kernel->Store->Expires->ExpireList.erase(it++);
                return true;
        }

        /* Key not found. */

        return false;
}

signed int ExpireManager::GetTIME(std::shared_ptr<Database> database, const std::string& key, const std::string& select)
{
        ExpireMap& expires = Kernel->Store->Expires->GetExpires();

        std::lock_guard<std::mutex> lg(ExpireManager::mute);

        for (ExpireMap::iterator it = expires.begin(); it != expires.end(); it++)
        {
                        if (it->second.key == key && it->second.select == select && it->second.database == database)
                        {
                                return it->first;
                        }
        }

        /* Not found, not expiring. */

        return -1;
}

signed int ExpireManager::Add(std::shared_ptr<Database> database, signed int schedule, const std::string& key, const std::string& select, bool epoch)
{
        if (schedule < 0)
        {
                return -1;
        }
        
        /* If entry already exists, we remove it and insert it again. */
        
        if (ExpireManager::GetTIME(database, key, select) > 0)
        {
              ExpireManager::Delete(database, key, select);
        }
        
        ExpireManager::mute.lock();
        
        time_t Now = Kernel->Now();
        ExpireEntry New;
        
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
        
        Kernel->Store->Expires->ExpireList.insert(std::make_pair(New.schedule, New));
        ExpireManager::mute.unlock();
        
        bprint(DONE, "Adding expire: %s", key.c_str());
        return New.schedule;
}

ExpireEntry ExpireManager::Find(std::shared_ptr<Database> database, const std::string& key, const std::string& select)
{
        std::lock_guard<std::mutex> lg(ExpireManager::mute);

        ExpireMap& current = Kernel->Store->Expires->GetExpires();

        for (ExpireMap::iterator it = current.begin(); it != current.end(); it++)
        {
                        if (it->second.key == key && it->second.select == select && it->second.database == database)
                        {
                                return it->second;
                        }
        }

        /* Not found, not expiring. */

        throw KernelException("ne");
}

ExpireMap& ExpireManager::GetExpires()
{
       std::lock_guard<std::mutex> lg(ExpireManager::mute);
       return this->ExpireList;
}

void ExpireManager::Flush(time_t TIME)
{
        /* 
         * Considering that Flush is an static function, this is the only valid to get expiring
         * entries. 
         */
           
        ExpireMap& expiring = Kernel->Store->Expires->GetExpires();

        if (!expiring.size())
        {
              return;
        }
        
        ExpireManager::mute.lock();

        for (ExpireMap::iterator it = expiring.begin(); it != expiring.end(); )
        {
              if (it->first >= TIME)
              {
                      break;
              }
              
              ExpireEntry entry = it->second;
              KeyHelper::Delete(Kernel->Clients->Global, entry.database, entry.select, entry.key, true);
              
              /* Now we remove the entry from the ExpireList Map. */
              
              Kernel->Store->Expires->ExpireList.erase(it++);
        }  
        
        ExpireManager::mute.unlock();
}

signed int ExpireManager::GetTTL(std::shared_ptr<Database> database, const std::string& key, const std::string& select)
{
      std::lock_guard<std::mutex> lg(ExpireManager::mute);
      return ExpireManager::GetTIME(database, key, select);
}

unsigned int ExpireManager::SelectReset(std::shared_ptr<Database> database, const std::string& select)
{
      /* Keeps track of deleted expires */
      
      unsigned int counter = 0;

      ExpireMap& expiring = Kernel->Store->Expires->GetExpires();

      if (!expiring.size())
      {
              return 0;
      }

      ExpireManager::mute.lock();

      for (ExpireMap::iterator it = expiring.begin(); it != expiring.end(); )
      {
            if (it->second.select == select && it->second.database == database)
            {
                       ExpireEntry entry = it->second;
                       ExpireHelper::Persist(Kernel->Clients->Global, entry.key, entry.select, entry.database);

                       Kernel->Store->Expires->ExpireList.erase(it++);
                       counter++;
            }
            else
            {
                       it++;
            }
      }  

      ExpireManager::mute.unlock();
      
      return counter;
}

void ExpireManager::Reset()
{
      ExpireMap& expiring = Kernel->Store->Expires->GetExpires();

      if (!expiring.size())
      {
              return;
      }

      ExpireManager::mute.lock();

      for (ExpireMap::iterator it = expiring.begin(); it != expiring.end(); )
      {
              ExpireEntry entry = it->second;
              ExpireHelper::Persist(Kernel->Clients->Global, entry.key, entry.select, entry.database);

              /* Now we remove the entry from the ExpireList Map. */

              Kernel->Store->Expires->ExpireList.erase(it++);
      }  

      ExpireManager::mute.unlock();
}

unsigned int ExpireManager::Count(std::shared_ptr<Database> database, const std::string& select)
{
        ExpireMap& expires = Kernel->Store->Expires->GetExpires();
        std::lock_guard<std::mutex> lg(ExpireManager::mute);
        
        unsigned int counter = 0;

        for (ExpireMap::iterator it = expires.begin(); it != expires.end(); it++)
        {
                ExpireEntry entry = it->second;

                if (entry.select == select && entry.database == database)       
                {
                       counter++;
                }
        }
        
        return counter;
}
        
        
