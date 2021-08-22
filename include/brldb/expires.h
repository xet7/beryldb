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

class ExpireEntry
{
    public:

        /* Expected time at which this timer will run */
        
        time_t schedule;
        
        /* Time added */ 
        
        time_t added;
        
        /* Epoch time */
        
        bool epoch;

        unsigned int secs;
        
        /* select used */
         
        unsigned int select;
        
        /* Key used */
        
        std::string key;
        
        /* Database in which this expire will run */
        
        std::shared_ptr<Database> database;

        ExpireEntry()
        {

        }
};

typedef std::multimap<time_t, ExpireEntry> ExpireMap;

class ExportAPI ExpireManager : public safecast<ExpireManager>
{
    public:
        
        /* Expires have mutex, as these are called inside db threads. */
        
        static std::mutex mute;
        
        /* Constructor */
        
        ExpireManager();

        /* Expires map */
        
        ExpireMap ExpireList;
        
        /* 
         * Flushes all pending expires.
         *
         * @parameters:
         *
         *         · time_t: Current time, as provided in mainloop.
         */
        
        void Flush(time_t TIME);
        
        /* 
         * Adds a new expire.
         * 
         * @parameters:
	 *
	 *         · Epoch: If this is an epoch (UNIX timestamp) entry.
	 * 
         * @return:
 	 *
         *         · -1 : Schedule already passed.
         *         · int: Time at which this timer will run.
         */            
         
        signed int Add(std::shared_ptr<Database> database, signed int schedule, const std::string& key, unsigned int select, bool epoch = false);

        /* 
         * Returns triggering time of a timer.
         * 
         * @return:
 	 *
         *         · -1: Expire not found.
         *         · int: time_t to expiring time.
         */          
           
        static signed int GetTIME(std::shared_ptr<Database> database, const std::string& key, unsigned int select);

        /* 
         * Deletes an entry from expires.
         * 
         * @return:
 	 *
         *         · True: Entry has been deleted. False: Not found.
         */            
         
        static bool Delete(std::shared_ptr<Database> database, const std::string& key, unsigned int select);

        /* 
         * Finds an expire.
         * 
         * @return:
 	 *
         *         · ExpireEntry: An static object to ExpireEntry.
         */    
                 
        ExpireEntry Find(std::shared_ptr<Database> database, const std::string& key, unsigned int select);
        
        /* 
         * Gets all expires.
	 * 
         * @return:
 	 *
         *         · ExpireMap: A map to all expires, keyed by time_t and a 
         *                      static object of a ExpireEntry.
         */          
           
        ExpireMap& GetExpires();        

        /* 
         * Time to live.
         * 
         * @return:
 	 *
         *         · Same as ExpireManager::GetTIME();
         */    
         
        signed int GetTTL(std::shared_ptr<Database> database, const std::string& key, unsigned int select);
        
        /* Clears everything inside ExpireList. */
        
        static void Reset();
        
        /* 
         * Destroys all elements in a given select.
         * 
         * @parameters:
	 *
	 *         · string	: Database name to destroy.
	 *	   · uint	: Select to destroy.
	 * 
         * @return:
 	 *
         *         · uint	: Expired removed (counter).
         */            
         
        static unsigned int SelectReset(const std::string& dbname, unsigned int select);

        void DatabaseDestroy(const std::string& dbname);

        static unsigned int DatabaseReset(const std::string& dbname);

        /* 
         * Counts all expires.
	 * 
         * @return:
 	 *
         *         · unsigned int: All expires.
         */    
                 
        unsigned int CountAll()
        {
            return this->ExpireList.size();
        }
        
        /* 
         * Returns all items on provided select.
         * 
         * @parameters:
	 *
	 *         · select: Select to look for.
	 * 
         * @return:
 	 *
         *         · unsigned int: Counter of all items found.
         */
                     
        unsigned int Count(const std::string& database, unsigned int select);

        /* 
         * This function should be called before a database is closing.
         * The main purpose of this function is to remove pending expires from the
         * flusher manager.
         * 
         * @parameters:
	 *
	 *         · dbname: Closing database.
         */            
         
        static void PreDBClose(const std::string& dbname);
};


