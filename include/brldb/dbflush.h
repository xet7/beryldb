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

#include "query.h"
#include "brldb/datathread.h"

/* Thread vector. */
       
typedef std::vector<DataThread*> DataThreadVector;

class ExportAPI DataFlush : public safecast<DataFlush>
{ 
      friend class StoreManager;

      private:

        /* Mute used to add notifications. */

        static std::mutex mute;
        
        /* Determines whether DataFlush should be delivering pendings. */
        
        std::atomic<bool> running;
        
        /* Vector containing all threads. */

        DataThreadVector threadslist;

        /* Flush processor */
        
        static void Flush(User* user, std::shared_ptr<QueryBase> signal);

        /* 
         * NotFound is called when a database was not found when processing
         * a signal.
         * 
         * @parameters:
	 *
	 *         · User: User to process.
	 * 
         * @print:
 	 *
         *         · Original requesting signal was not found.
         */    
         
         static void NotFound(User* user, std::shared_ptr<QueryBase> signal);

        /* 
         * Format is not valid.
         * 
         * @print:
         *     
         *        · INVALID_FORMAT
         */
             
         static void InvalidFormat(User* user, std::shared_ptr<QueryBase> signal);

        /* 
         * Missing arguments.
         * 
         * @print:
         *     
         *        · MIS_ARGS
         */

         static void MissArgs(User* user, std::shared_ptr<QueryBase> signal);
         
        /* 
         * Unable to modify destination key.
         * 
         * @print:
         *     
         *        · INVALID_FORMAT (DATA_TYPE).
         */

         static void InvalidType(User* user,  std::shared_ptr<QueryBase> signal);

        /* 
         * Request has failed, due to a database failure.
         * 
         * @print:
         *     
         *        · DB_NULL.
         */

         static void StatusFailed(User* user, std::shared_ptr<QueryBase> signal);

         static void BatchFailed(User* user, std::shared_ptr<QueryBase> signal);

        /* 
         * Entry already is defined.
         * 
         * @print:
         *     
         *        · ENTRY_DEFINED
         */

         static void EntryExists(User* user, std::shared_ptr<QueryBase> signal);

        /* 
         * Unable to write data.
         * 
         * @print:
         *     
         *        · PROCESS_FALSE
         */

         static void UnableWrite(User* user, std::shared_ptr<QueryBase> signal);

        /* 
         * Entry tried to be modify is not numeric.
         * 
         * @print:
         *     
         *        · PROCESS_NOT_NUM
         */

         static void NotNumeric(User* user, std::shared_ptr<QueryBase> signal);

        /* 
         * Range is invalid.
         * 
         * @print:
         *     
         *        · INVALID_RANGE
         */

         static void InvalidRange(User* user, std::shared_ptr<QueryBase> signal);

        /* 
         * Entry is expiring.
         * 
         * @print:
         *     
         *        · ENTRY_EXPIRES
         */

         static void EntryExpires(User* user, std::shared_ptr<QueryBase> signal);

        /* 
         * Entry is not expiring.
 	 *
 	 * @print:
 	 *
         *         · ENTRY_NOT_EXPIRING
         */    
         
         static void EntryNOExpires(User* user, std::shared_ptr<QueryBase> signal);
        
         /* Results from the processing threads. */
        
         static void GetResults();

         /* Gets pending queries by iterating all users. */
 
         static void GetPending();

    public:
    
         static std::mutex query_mute;

         /* Flush constructor, should should be set to OK after this. */
        
         DataFlush();

        /* 
         * Process a new query. When this function is called, 'signal'
         * query will be posted to the thread processor.
         *
         * @parameters:
	 *
	 *         · signal: Signal to process.
         */             
         
         static void Process(User* user, std::shared_ptr<QueryBase> signal);
        
         /* Pauses this->running */
        
         void Pause();
        
         /* Resumes this->running */
        
         void Resume();
        
         /* Current status for this->running */
        
         bool Status();
    
         /* Clears notifications and pending queries. */
        
         static void ResetAll();

        /* 
         * Adds a new notification to the notification vector.
         * 
         * @parameters:
	 *
	 *         · result: Resulting query.
         */    
         
         static void AttachResult(std::shared_ptr<QueryBase> result);

         /* Called in the mainloop, used to dispatch notifications and pending queries. */
        
         static void Process();

         /* Close all threads */
        
         void CloseThreads();     
        
         /* Returns vector of threads. */
        
         DataThreadVector& GetThreads()
         {
                return this->threadslist;
         }
        
        /* 
         * Counts active threads.
	 * 
         * @return:
 	 *
         *         · uint: Counter.
         */            
         
         unsigned int CountThreads()
         {
                return this->threadslist.size();
         }
         
         /* Cleans up threadslist. */
         
         void EraseAll()
         { 
                return this->threadslist.clear();
         }
};