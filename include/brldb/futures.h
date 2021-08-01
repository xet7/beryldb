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

class ExportAPI FutureManager : public safecast<FutureManager>
{
      public:

        static std::mutex mute;

        /* Constructor */

        FutureManager();
        
        /* Future list. */

        FutureMap FutureList;

        /* 
         * Flushes all pending futures. 
         *
         * @parameters:
         *
         *         · time_t: Current time, as provided in mainloop.
         */

        void Flush(time_t TIME);

        /* Returns triggering time for a future. */

        static signed int GetTIME(std::shared_ptr<Database> database, const std::string& key, const std::string& select);

        /* 
         * Deletes a given future.
         * 
	 * 
         * @return:
 	 *
         *         · True: Future removed.
         *         · False: Not found or unable to remove.
         */    
         
        static bool Delete(std::shared_ptr<Database> database, const std::string& key, const std::string& select);

        static void Reset();

        /* 
         * Finds a future.
         * 
         * @parameters:
	 *
	 *         · database: Database to look for.
	 *         · key: Key to look up.
	 *         · select: Select to use.
	 * 
         * @return:
 	 *
         *         · FutureEntry: An static object to a FutureEntry class. 
         */    
         
        static FutureEntry Find(std::shared_ptr<Database> database, const std::string& key, const std::string& select);

        /* Exercises an entry. */

        static bool Exec(std::shared_ptr<Database> database, const std::string& key, const std::string& select);

        /* Adds a new future. */

        signed int Add(std::shared_ptr<Database> database, signed int schedule, const std::string& key, const std::string& value, const std::string& select, bool epoch);

        /* 
         * Pending time on a future to be executed.
         * 
         * @return:
 	 *
         *         · int: Seconds remaining for this entry to be executed.
         */    
         
        signed int GetTTE(std::shared_ptr<Database> database, const std::string& key, const std::string& select);

        /* 
         * Returns a value from FutureMap.
         * 
         * @parameters:
	 *
	 *         · database: Database to lookup.
	 *         · key: Key to find.
	 *         · select: Select to utilize.
	 * 
         * @return:
 	 *
         *         · tuple: int (0 if not found, 1 if found), string found.
         */    
         
        static std::tuple<int, std::string> GetVal(std::shared_ptr<Database> database, const std::string& key, const std::string& select);
            
        /* 
         * Obtain all futures in the FutureList.
         * 
         * @return:
 	 *
         *         · FutureMap: Map of futures.
         */                
         
        FutureMap& GetFutures();        

        /* 
         * Counts all items in FutureList.
	 * 
         * @return:
         *  · unsigned int: Item size.
         */    
         
        unsigned int CountAll()
        {
            return this->FutureList.size();
        }

        unsigned int Count(std::shared_ptr<Database> database, const std::string& select);

        unsigned int SelectReset(const std::string& dbname, const std::string& select);      

        /* 
         * Removes all entries from a given database.
         * 
         * @parameters:
	 *
	 *         · dbname: Database name to remove entries from.
	 * 
         * @return:
 	 *
         *         · uint: Removed elements.
         */            
         
        unsigned int DatabaseReset(const std::string& dbname);

        void DatabaseDestroy(const std::string& dbname);

        /* 
         * This function should be called before a database is closing.
         * The main purpose of this function is to remove pending futures from the
         * flusher manager.
         * 
         * @parameters:
         *
         *         · dbname: Closing database.
         */
        
        static void PreDBClose(const std::string& dbname);
};