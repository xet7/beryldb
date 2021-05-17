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

#pragma once

#include <mutex>

class FutureEntry
{
  public:
  
        time_t schedule;
        time_t added;

        bool epoch;

        unsigned int secs;
        std::string select;
        std::string key;
        std::string value;
        
        std::shared_ptr<Database> database;

        FutureEntry()
        {
        
        }
};

typedef std::multimap<time_t, FutureEntry> FutureMap;

class Externalize FutureManager : public safecast<FutureManager>
{
  public:
  
      static std::mutex mute;
  
      FutureManager();
      
      FutureMap FutureList;
      
      void Flush(time_t TIME);

      /* Returns triggering time for a future. */
        
      static signed int TriggerTIME(const std::string& key, const std::string& select);

      static bool Delete(const std::string& key, const std::string& select);
      
      static void Reset();

      FutureEntry Find(const std::string& key, const std::string& select);

      /* Adds a new future. */
      
      signed int Add(signed int schedule, const std::string& key, const std::string& value, const std::string& select, bool epoch);

      /* Time to exercise. */
      
      signed int GetTTE(const std::string& key, const std::string& select);
             
      FutureMap& GetFutures();        
             
      /* Counts all futures. */
        
      unsigned int CountAll()
      {
            return this->FutureList.size();
      }

      
};