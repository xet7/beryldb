
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
        std::shared_ptr<Database> database;

        FutureEntry()
        {
        
        }
};

typedef std::multimap<time_t, FutureEntry> FutureMap;

class FutureManager : public safecast<FutureManager>
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

      signed int GetTTE(const std::string& key, const std::string& select);
             
      FutureMap& GetFutures();        
             
      /* Counts all futures. */
        
      unsigned int CountAll()
      {
            return this->FutureList.size();
      }

      
};