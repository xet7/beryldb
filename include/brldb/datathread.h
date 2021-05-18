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
      ThreadMsg(THR_CMD i, std::shared_ptr<query_base> m) : msg(m)
      { 
            id = i; 
      }

      THR_CMD id;
      std::shared_ptr<query_base> msg;
};

class DataThread
{   
    private:

        std::unique_ptr<std::thread> m_thread;
        
        /* Queue item to attach. */
        
        std::queue<std::shared_ptr<ThreadMsg>> queue;
        std::mutex m_mutex;
        std::condition_variable m_cv;
        
        /* Keeps track of busy status */
      
        std::atomic<bool> busy;
        
    public:

        DataThread();

        void SetStatus(bool flag);

        bool IsBusy();
    
        /* A mainloop for a datathread. */
        
        void Process();

        std::thread::id Create();

        /* Exists thread */
        
        void Exit();
        
        /* Destructor */
        
        ~DataThread();
        
        /* Ads a new request to the processor. */
        
        void Post(std::shared_ptr<query_base> query);

        /* Removes all items in this->queue */
        
        void clear()
        {
              std::queue<std::shared_ptr<ThreadMsg>> empty;
              std::swap(this->queue, empty);
        }        
};