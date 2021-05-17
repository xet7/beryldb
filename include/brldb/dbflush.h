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

#include "query.h"
#include "brldb/datathread.h"

class Externalize DataFlush : public safecast<DataFlush>
{ 
      friend class StoreManager;

      private:

        /* Mute used to add notifications. */

        static std::mutex mute;
        
        /* Thread vector. */
       
        typedef std::vector<DataThread*> DataThreadVector;

        /* Determines whether DataFlush should be delivering pendings. */
        
        std::atomic<bool> running;
        
        /* Vector containing all threads. */

        DataThreadVector threadslist;

        /* Flush processor */
        
        static void Flush(User* user, std::shared_ptr<query_base> signal);
        
        /* Results from the processing threads. */
        
        static void GetResults();

        /* Gets pending queries by iterating all users. */

        static void GetPending();

        /* mute that keep track of flushing status. */

        std::atomic<bool> flushmute;

        /* Format locker. */
        
        static std::mutex wlock;
        
        /* Locked words. */
        
        std::vector<std::string> locks;
   
   public:
           
        /* Lock for operations. */
        
        std::atomic<bool> opmute;

        /* Locks a entry. */
        
        void Lock(const std::string& format);

        /* Unlocks an entry. */
        
        void Unlock(const std::string& format);
        
        /* Checks if an entry is locked. */

        bool IsLocked(const std::string& format);

        /* Flush constructor, should should be set to OK after this. */
        
        DataFlush();

        static void Process(User* user, std::shared_ptr<query_base> signal);
        
        /* Pauses this->running */
        
        void Pause();
        
        /* Resumes this->running */
        
        void Resume();
        
        /* Current status for this->running */
        
        bool Status();
    
        /* Clears notifications and pending queries. */
        
        static void ResetAll();

        /* Adds a new notification */

        static void AttachResult(std::shared_ptr<query_base> result);

        /* Called in the mainloop, used to dispatch notifications and pending queries. */
        
        static void DispatchAll();

        /* Close all threads */
        
        void CloseThreads();     
        
        /* Returns vector of threads. */
        
        DataThreadVector& GetThreads()
        {
                return this->threadslist;
        }
        
        void EraseAll()
        {
                return this->threadslist.clear();
        }
};