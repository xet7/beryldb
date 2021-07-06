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

#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "brldb/query.h"

enum THR_CMD
{
        PROC_EXIT_THREAD = 1,
        PROC_SIGNAL = 2,
};

struct ThreadMsg
{
      ThreadMsg(THR_CMD i, std::shared_ptr<QueryBase> m) : msg(m)
      { 
            id = i; 
      }

      THR_CMD id;
      std::shared_ptr<QueryBase> msg;
};

class DataThread
{   
    private:

        /* Keeps this thread safe. */
        
        std::mutex m_mutex;
        
        std::unique_ptr<std::thread> m_thread;
        
        /* Queue item to attach. */
        
        std::queue<std::shared_ptr<ThreadMsg>> queue;
        
        /* Conditional variable for thread. */
        
        std::condition_variable m_cv;
        
        /* Keeps track of busy status */
      
        std::atomic<bool> busy;
        
    public:
    
        /* Thread constructor. */
        
        DataThread();

        void SetStatus(bool flag);

        /* 
         * Checks if thread is busy.
         
         * @return:
 	 *
         *         · True: Thread busy.
         *         · False: Thread available.
         */    
         
        bool IsBusy();
    
        /* A mainloop for a datathread. */
        
        void Process();
        
        /* 
         * Creates a new thread.
         * 
         * @return:
 	 *
         *         · thread::id: Thread id.
         */    

        std::thread::id Create();

        /* Exists thread */
        
        void Exit();
        
        /* Destructor */
        
        ~DataThread();

        /* 
         * Adds a new query to the thread manager.
         * 
         * @parameters:
	 *
	 *         · query: Thread to process.
         */            
        
        void Post(std::shared_ptr<QueryBase> query);

        /* Removes all items in this->queue */
        
        void clear()
        {
              std::queue<std::shared_ptr<ThreadMsg>> empty;
              std::swap(this->queue, empty);
        }        
};