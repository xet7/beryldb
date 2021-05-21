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
#include "brldb/database.h"
#include "brldb/datathread.h"
#include "brldb/dbnumeric.h"
#include "brldb/dbflush.h"
#include "algo.h"

std::mutex DataFlush::mute;

std::mutex DataFlush::wlock;

void DataFlush::Pause()
{
      this->running = false;
}

void DataFlush::Resume()
{
      this->running = true;
}

bool DataFlush::Status()
{
      return this->running;
}

DataFlush::DataFlush() : running(false), flushmute(false), opmute(false)
{

}

void DataFlush::AttachResult(std::shared_ptr<query_base> signal)
{
      std::lock_guard<std::mutex> lg(DataFlush::mute);
      signal->user->notifications.push_back(signal);
}

void DataFlush::Lock(const std::string& format)
{
      std::lock_guard<std::mutex> lg(DataFlush::wlock);
      this->locks.push_back(format);
}

void DataFlush::Unlock(const std::string& format)
{
      std::lock_guard<std::mutex> lg(DataFlush::wlock);
      std::vector<std::string>::iterator it = std::find(this->locks.begin(), this->locks.end(), format);
      
      if (it != this->locks.end())
      {
            this->locks.erase(it);
      }
}

bool DataFlush::IsLocked(const std::string& format)
{
      std::lock_guard<std::mutex> lg(DataFlush::wlock);
      std::vector<std::string>::iterator it = std::find(this->locks.begin(), this->locks.end(), format);
      
      if (it != this->locks.end())
      {
            return true;
      }
      
      return false;
}

void DataFlush::GetResults()
{
      const UserMap& users = Kernel->Clients->GetInstances();
      
      for (UserMap::const_iterator u = users.begin(); u != users.end(); ++u)
      {
                  User* user = u->second;

                  if (user == NULL || user->IsQuitting())
                  {
                        continue;
                  }

                  DataFlush::mute.lock();
                  
                  /* Nothing to iterate if user has no pending notifications. */

                  if (user->notifications.size())
                  {
                        std::shared_ptr<query_base> signal = user->notifications.front();
                        
                        if (signal->Lock)
                        {
                                continue;
                        }
                        
                        /* Removes signal from queue. */
                        
                        user->notifications.pop_front();
                        
                        /* Releases dataflush mute. */
                        
                        DataFlush::mute.unlock();
                        
                        switch (signal->access)
                        {
                             case DBL_NOT_FOUND:
                             
                                     DataFlush::NotFound(user, signal);
                             
                             break;
                             
                             case DBL_MISS_ARGS:
                             
                                    DataFlush::MissArgs(user, signal);
                             
                             break;
                             
                             case DBL_ENTRY_EXISTS:
        
                                     DataFlush::EntryExists(user, signal);
                             
                             break;
                             
                             case DBL_STATUS_BROKEN:
                             
                                     DataFlush::StatusFailed(user, signal);
                            break;
                            
                            default:
                             
                               DataFlush::Flush(user, signal);
                        }
                        
                        if (user)
                        {
                                user->SetLock(false);
                        }

                        signal.reset();
                  }
                  else
                  {
                        DataFlush::mute.unlock();
                  }
      }
}

void DataFlush::DispatchAll()
{
      /* We should not dispatch anything if we are paused. */
      
      if (!Kernel->Store->Flusher->Status())  
      {
            return;   
      }

      DataFlush::GetResults();
      DataFlush::GetPending();
}

void DataFlush::GetPending()
{
      /* Before processing user pendings, we must check global. */
      
      if (Kernel->Clients->Global->pending.size())
      {
               Process(Kernel->Clients->Global, Kernel->Clients->Global->pending.front());
               return;
      }

      const UserMap& users = Kernel->Clients->GetInstances();
      
      for (UserMap::const_iterator u = users.begin(); u != users.end(); ++u)
      {
               User* user = u->second;

               unsigned int pending = user->pending.size();

               if ((user && !user->IsQuitting()) && (!pending || user->IsLocked()))
               {
                    continue;
               }
               
               if (pending > 1 && !Kernel->Lock)
               {
                        Kernel->Interval = 0;
               }

               Process(user, user->pending.front());
      }
}

void DataThread::SetStatus(bool flag)
{
      this->busy = flag;
}

bool DataThread::IsBusy()
{
      return this->busy;
}

void DataFlush::Process(User* user, std::shared_ptr<query_base> signal)
{
      if (!signal || !Kernel->Store->Flusher->Status())
      {
            return;
      }

      user->SetLock(true);

      /* Only one operation at a time. */
      
      if (signal->type == DBL_TYPE_OP)
      {
            if (Kernel->Store->Flusher->opmute)
            {
                    return;
            }
            
            Kernel->Store->Flusher->opmute = true;
      }
  
      DataThreadVector& Threads = Kernel->Store->Flusher->GetThreads();
      
      /* Thread to use. */
      
      DataThread *ToUse = NULL;
      
      for (DataThreadVector::iterator iter = Threads.begin(); iter != Threads.end(); ++iter)
      {
           DataThread* thread = *iter;
           
           /* First, we attempt to find an unused thread. */
           
           if (!thread->IsBusy())
           {
                 ToUse = thread;
                 
                 /* No need to continue looking for a thread. */
                 
                 break; 
           }
      }
      
      /* Checks whether we found a not-busy thread. */
      
      if (!ToUse)
      {
            /* We find a random thread to attach this signal to. */
      
            ToUse = *random_item(Threads.begin(), Threads.end());
      }

      /* Unable to find any thread at all. */
      
      if (!ToUse)
      {
            user->SetLock(true);
            return;
      }

      user->pending.pop_front();

      /* Let's start processing this signal. */
      
      ToUse->Post(signal);
}

void DataFlush::ResetAll()
{
      Kernel->Store->Flusher->Pause();

      if (Kernel->Clients->Global)
      {
          Kernel->Clients->Global->pending.clear();
          Kernel->Clients->Global->notifications.clear();
      }
      
      /* Clear up pending notifications. */

      const UserMap& users = Kernel->Clients->GetInstances();
      
      for (UserMap::const_iterator u = users.begin(); u != users.end(); ++u)
      {           
              User* user = u->second;
            
              if (user == NULL || user->IsQuitting())
              {
                     continue;
              }

              DataFlush::mute.lock();
              user->notifications.clear();
              user->pending.clear();
              DataFlush::mute.unlock();
      }

      Kernel->Store->Expires->Reset();
      Kernel->Store->Flusher->Resume();
}

DataThread::DataThread() : m_thread(nullptr), busy(false)
{

}

void DataThread::Exit()
{
      if (!m_thread)
      {
           return;
      }

      std::shared_ptr<ThreadMsg> Input(new ThreadMsg(PROC_EXIT_THREAD, 0));
      {
                std::lock_guard<std::mutex> lock(m_mutex);
                queue.push(Input);
                m_cv.notify_one();
      }

      m_thread->join();
      m_thread = nullptr;
}

void DataThread::Post(std::shared_ptr<query_base> query)
{
      if (!m_thread)
      {
            return;
      }
      
      std::shared_ptr<ThreadMsg> Input(new ThreadMsg(PROC_SIGNAL, query));

      std::unique_lock<std::mutex> lk(m_mutex);
      queue.push(Input);
      m_cv.notify_one();
}

std::thread::id DataThread::Create()
{
      if (!m_thread)
      {
           m_thread = std::unique_ptr<std::thread>(new std::thread(&DataThread::Process, this)); 
      }

      return m_thread->get_id();
}

void DataThread::Process()
{
        while (true)
        {
              std::shared_ptr<ThreadMsg> signal;
              {
                      std::unique_lock<std::mutex> lk(m_mutex);

                      while (queue.empty())
                      {
                            m_cv.wait(lk);
                      }

                      if (queue.empty())
                      {
                            continue;
                      }

                      signal = queue.front();
              }

              /* Indicates this thread is busy. */

              this->SetStatus(true);
              
              if (Kernel->Lock)
              {
                    usleep(10);
              }

              if (!Kernel->Store->Flusher->Status())
              {
                    continue;
              }

              switch (signal->id)
              {
                    case PROC_EXIT_THREAD:
                    {
                          this->clear();
                          return;
                    }
                    
                    case PROC_SIGNAL:
                    {
                          std::shared_ptr<query_base> request = signal->msg;
                          
                          /* Let's release the locked user. */

                          if (request->user && !request->user->IsQuitting())
                          {
                                /* OK */
                                
                          }
                          else
                          {
                               /* In case lock status gets locked. */
                               
                               if (request->user)
                               {
                                     request->user->SetLock(false);
                               }
                               
                               queue.pop();
                               
                               signal.reset();
                               signal = nullptr;
                               
                               continue;
                          }

                          if (!request->format.empty())
                          {
                                if (Kernel->Store->Flusher->IsLocked(request->format))
                                {
                                       continue;
                                }
                                
                                queue.pop();
                                
                                /* Locks this format. */

                                Kernel->Store->Flusher->Lock(request->format);
                          }
                          else
                          {
                                queue.pop();
                          }

                          request->Lock = true;

                          /* Executes signal request. */
                          
                          if (request->Check())
                          {
                              request->Run();
                          }
                          
                          request->Lock = false;

                          if (!request->format.empty())
                          {
                              /* Unlocker. */

                              Kernel->Store->Flusher->Unlock(request->format);
                          }
                          
                          if (request->access != DBL_INTERRUPT && !request->quiet)
                          {
                                 /* Adds result to the pending notification list. */
 
                                 DataFlush::AttachResult(request);
                          }
                          
                          break;
                    }

                    default:
                    {
                           break;
                    }
              }
              
              /* Thread is no longer busy. */

              this->SetStatus(false);
              
        }
}

DataThread::~DataThread()
{

}

void DataFlush::CloseThreads()
{
      DataThreadVector& Threads = Kernel->Store->Flusher->GetThreads();

      unsigned int counter = 0;
      
      for (DataThreadVector::iterator iter = Threads.begin(); iter != Threads.end(); ++iter)
      {
             DataThread* thread = *iter;
             thread->Exit();
             counter++;
      }     
      
      Kernel->Store->Flusher->EraseAll();
      bprint(INFO, "Threads closed: %u", counter);
      slog("DATABASE", LOG_VERBOSE, "Threads closed: %u", counter);
}