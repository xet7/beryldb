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
        PROC_EXIT_THREAD = 1,	/* Request to exit thread */
        PROC_SIGNAL 	 = 2,   /* Request to process a QueryBase signal */
};

struct ThreadMsg
{
        THR_CMD id;
        std::shared_ptr<QueryBase> msg;

        ThreadMsg(THR_CMD identifier, std::shared_ptr<QueryBase> message) : msg(message)
        { 
             id = identifier; 
        }
};

class DataThread
{   
    private:

        /* Keeps this thread safe. */
        
        std::mutex m_mutex;
        
        std::unique_ptr<std::thread> handler;
        
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
         *
         * @return:
 	 *
 	 *       - bool
 	 *
         *         · true	: Thread busy.
         *         · false	: Thread available.
         */    
         
        bool IsBusy();
    
        /* A mainloop for a datathread. */
        
        void Process();
        
        /* 
         * Creates a new thread.
         * 
         * @return:
 	 *
         *         · thread::id  : Thread id.
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
	 *         · QueryBase   : Thread to process.
         */            
        
        void Post(const std::shared_ptr<QueryBase> query);

        /* Removes all items in this->queue */
        
        void Clear();
};