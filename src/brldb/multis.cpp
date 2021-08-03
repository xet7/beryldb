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

#include <random>

#include "beryl.h"
#include "engine.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "brldb/expires.h"
#include "brldb/functions.h"
#include "brldb/multimap_handler.h"
#include "helpers.h"
#include "brldb/dbmanager.h"

void mseek_query::Run()
{
       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;
       
       RocksData query_result = this->Get(this->dest);
       
       std::shared_ptr<MultiMapHandler> handler = MultiMapHandler::Create(query_result.value);
       
       Args result = handler->Find(this->value);

       Args result_return;
       
       for (Args::iterator i = result.begin(); i != result.end(); ++i)
       {
                std::string hesh_as_string = *i;
                
                if (this->limit != -1 && ((signed int)total_counter >= this->offset))
                {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    result_return.push_back(hesh_as_string);
             
                                    if (aux_counter % ITER_LIMIT == 0)
                                    {
                                                std::shared_ptr<mseek_query> request = std::make_shared<mseek_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = ++tracker;
                                                request->VecData = result_return;
                                                result.clear();
                                                request->SetOK();
                                                DataFlush::AttachResult(request);
                                      }
                                      
                                      if (aux_counter == (unsigned int)limit)
                                      {
                                                break;               
                                      }
                             }
                }
                else if (limit == -1)
                {
                             aux_counter++;
                             result_return.push_back(hesh_as_string);
            
                             if (aux_counter % ITER_LIMIT == 0)
                             {
                                        std::shared_ptr<mseek_query> request = std::make_shared<mseek_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = ++tracker;
                                        request->VecData = result_return;
                                        result.clear();
                                        request->SetOK();
                                        DataFlush::AttachResult(request);
                             }
                }
                         
                total_counter++;
    }

     this->subresult = ++tracker;
     this->partial = false;
     this->counter = total_counter;
     this->VecData = result_return;
     this->SetOK();
}

void mseek_query::Process()
{
        if (this->subresult == 1)
        {
               Dispatcher::JustAPI(user, BRLD_START_LIST);                 
        }

        for (Args::iterator i = this->VecData.begin(); i != this->VecData.end(); ++i)
        {            
               std::string item = *i;
               user->SendProtocol(BRLD_ITEM, item.c_str());
        }

        if (!this->partial)
        {
               Dispatcher::JustAPI(user, BRLD_END_LIST);                 
        }
}

void mdel_query::Run()
{
       RocksData result = this->Get(this->dest);

       std::shared_ptr<MultiMapHandler> handler = MultiMapHandler::Create(result.value);
       handler->Remove(this->value);
       
       if (handler->Count() > 0)
       {
               if (this->Write(this->dest, handler->as_string()))
               {
                       this->SetOK();
               }
               else
               {
                       access_set(DBL_UNABLE_WRITE);
                       return;
               }
       }
       else
       {
               this->Delete(this->dest);
       } 
       
       this->SetOK();
}

void mdel_query::Process()
{
        user->SendProtocol(BRLD_OK, PROCESS_OK);
}


void mset_query::Run()
{
       if (this->value.empty() || this->hesh.empty())
       {
               this->access_set(DBL_MISS_ARGS);
               return;
       }

       RocksData result = this->Get(this->dest);

       std::shared_ptr<MultiMapHandler> handler;

       if (!result.status.ok())
       {
               handler = std::make_shared<MultiMapHandler>();
               handler->Add(this->hesh, this->value);

               if (handler->GetLast() == HANDLER_MSG_OK)
               {
                    if (this->Write(this->dest, handler->as_string()))
                    {
                             this->SetOK();
                    }
                    else
                    {
                          access_set(DBL_UNABLE_WRITE);
                          return;
                    }
               }

               this->SetOK();
               return;
       }

       handler = MultiMapHandler::Create(result.value);
       handler->Add(this->hesh, this->value);

       if (handler->GetLast() == HANDLER_MSG_OK)
       {
            this->Write(this->dest, handler->as_string());
       }

       this->SetOK();
}

void mset_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}

void mkeys_query::Run()
{
       Args result;

       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
       std::string rawmap;
       
       unsigned int aux_counter = 0;
       unsigned int total_counter = 0;
       unsigned int tracker = 0;
       
       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status() || this->database->IsClosing())

                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }

                rawmap = it->key().ToString();
                
                engine::colon_node_stream stream(rawmap);
                std::string token;

                unsigned int strcounter = 0;
                bool skip = false;

                std::string key_as_string;
                std::string aux;
                
                while (stream.items_extract(token))
                {
                        if (skip)
                        {
                               break;
                        }
                        
                        switch (strcounter)
                        {
                             case 0:
                             {
                                  key_as_string = to_string(token);

                                  if (!Daemon::Match(key_as_string, this->key))                                  
                                  {
                                         skip = true;
                                  }
                             }

                             break;

                             case 1:
                             {
                                  if (this->select_query != token)
                                  {
                                       skip = true;
                                  }
                             }

                             break;
                             
                             case 2:
                             {
                                    if (this->base_request != token)
                                    {
                                        skip = true;
                                    }
                             }

                             break;
                      
                             default:
                             {
                                 break;   
                             }
                        }

                        strcounter++;
                }

                if (skip)
                {
                        continue;
                }

                if (this->limit != -1 && ((signed int)total_counter >= this->offset))
                {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    result.push_back(key_as_string);
             
                                    if (aux_counter % ITER_LIMIT == 0)
                                    {
                                                std::shared_ptr<mkeys_query> request = std::make_shared<mkeys_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = ++tracker;
                                                request->VecData = result;
                                                result.clear();
                                                request->SetOK();
                                                DataFlush::AttachResult(request);
                                      }
                                      
                                      if (aux_counter == (unsigned int)limit)
                                      {
                                                break;               
                                      }
                             }
                }
                else if (limit == -1)
                {
                             aux_counter++;
                             result.push_back(key_as_string);
            
                             if (aux_counter % ITER_LIMIT == 0)
                             {
                                        std::shared_ptr<mkeys_query> request = std::make_shared<mkeys_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = ++tracker;
                                        request->VecData = result;
                                        result.clear();
                                        request->SetOK();
                                        DataFlush::AttachResult(request);
                             }
                }
                         
                total_counter++;
       }  
       
       this->subresult = ++tracker;
       this->partial = false;
       this->counter = aux_counter;
       this->VecData = result;
       this->SetOK();
}              

void mkeys_query::Process()
{
        if (this->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s", "Key"));
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s", Dispatcher::Repeat("―", 30).c_str()));
        }
        
        for (Args::iterator i = this->VecData.begin(); i != this->VecData.end(); ++i)
        {            
                 std::string item = *i;
                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s", item.c_str()), Daemon::Format("%s", item.c_str()));
        }

        if (!this->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);                 
        }
}

void mget_query::Run()
{
       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;
       
       RocksData query_result = this->Get(this->dest);
       
       if (!query_result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }
       
       std::shared_ptr<MultiMapHandler> handler = MultiMapHandler::Create(query_result.value);
       
       DualMMap result = handler->GetList();

       Args result_return;
       
       for (DualMap::iterator i = result.begin(); i != result.end(); ++i)
       {
                std::string hesh_as_string = i->first;
                
                if (this->flags == QUERY_FLAGS_CORE)
                {
                       result_return.push_back(hesh_as_string);
                       total_counter++;
                       continue;
                }
                
                if (this->flags == QUERY_FLAGS_COUNT)
                {
                      total_counter++;
                      continue;
                }
                
                if (this->limit != -1 && ((signed int)total_counter >= this->offset))
                {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    result_return.push_back(hesh_as_string);
             
                                    if (aux_counter % ITER_LIMIT == 0)
                                    {
                                                std::shared_ptr<mget_query> request = std::make_shared<mget_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = ++tracker;
                                                request->VecData = result_return;
                                                result.clear();
                                                request->SetOK();
                                                DataFlush::AttachResult(request);
                                      }
                                      
                                      if (aux_counter == (unsigned int)limit)
                                      {
                                                break;               
                                      }
                             }
                }
                else if (limit == -1)
                {
                             aux_counter++;
                             result_return.push_back(hesh_as_string);
            
                             if (aux_counter % ITER_LIMIT == 0)
                             {
                                        std::shared_ptr<mget_query> request = std::make_shared<mget_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = ++tracker;
                                        request->VecData = result_return;
                                        result.clear();
                                        request->SetOK();
                                        DataFlush::AttachResult(request);
                             }
                }
                         
                total_counter++;
    }

     this->subresult = ++tracker;
     this->partial = false;
     this->counter = total_counter;
     this->VecData = result_return;
     this->SetOK();
}

void mget_query::Process()
{
        if (this->flags == QUERY_FLAGS_COUNT)
        {
               user->SendProtocol(BRLD_OK, convto_string(this->counter));
               return;
        }

        if (this->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s", "Key"));
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s", Dispatcher::Repeat("―", 30).c_str()));
        }

        for (Args::iterator i = this->VecData.begin(); i != this->VecData.end(); ++i)
        {
                 std::string item = *i;
                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s", item.c_str()), Daemon::Format("%s", item.c_str()));
        }

        if (!this->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);
        }
}

void mrepeats_query::Run()
{
       RocksData result = this->Get(this->dest);

       std::shared_ptr<MultiMapHandler> handler = MultiMapHandler::Create(result.value);
       this->response = convto_string(handler->Repeats(this->value));
       this->SetOK();
}

void mrepeats_query::Process()
{
        user->SendProtocol(BRLD_OK, this->response.c_str());
}


void mvals_query::Run()
{
       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;
       
       RocksData query_result = this->Get(this->dest);
       
       std::shared_ptr<MultiMapHandler> handler = MultiMapHandler::Create(query_result.value);
       
       Args result = handler->GetValues();

       Args result_return;
       
       for (Args::iterator i = result.begin(); i != result.end(); ++i)
       {
                std::string hesh_as_string = (*i);
                
                if (this->limit != -1 && ((signed int)total_counter >= this->offset))
                {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    result_return.push_back(hesh_as_string);
             
                                    if (aux_counter % ITER_LIMIT == 0)
                                    {
                                                std::shared_ptr<hvals_query> request = std::make_shared<hvals_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = ++tracker;
                                                request->VecData = result_return;
                                                result.clear();
                                                request->SetOK();
                                                DataFlush::AttachResult(request);
                                      }
                                      
                                      if (aux_counter == (unsigned int)limit)
                                      {
                                                break;               
                                      }
                             }
                }
                else if (limit == -1)
                {
                             aux_counter++;
                             result_return.push_back(hesh_as_string);
            
                             if (aux_counter % ITER_LIMIT == 0)
                             {
                                        std::shared_ptr<hvals_query> request = std::make_shared<hvals_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = ++tracker;
                                        request->VecData = result_return;
                                        result.clear();
                                        request->SetOK();
                                        DataFlush::AttachResult(request);
                             }
                }
                         
                total_counter++;
    }

     this->subresult = ++tracker;
     this->partial = false;
     this->counter = total_counter;
     this->VecData = result_return;
     this->SetOK();
}

void mvals_query::Process()
{
        if (this->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s", "Key"));
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s", Dispatcher::Repeat("―", 30).c_str()));
        }
        
        for (Args::iterator i = this->VecData.begin(); i != this->VecData.end(); ++i)
        {            
                 std::string item = *i;
                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s", item.c_str()), Daemon::Format("%s", item.c_str()));
        }

        if (!this->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);                 
        }
}

void mgetall_query::Run()
{
       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;
       
       RocksData query_result = this->Get(this->dest);
       
       std::shared_ptr<MultiMapHandler> handler = MultiMapHandler::Create(query_result.value);
       
       DualMMap result = handler->GetAll();

       std::multimap<std::string, std::string> result_return;
       
       for (DualMMap::iterator i = result.begin(); i != result.end(); ++i)
       {
                std::string vkey = i->first;
                std::string vvalue = i->second;
                
                if (this->limit != -1 && ((signed int)total_counter >= this->offset))
                {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    result_return.insert(std::make_pair(vkey, vvalue));
             
                                    if (aux_counter % ITER_LIMIT == 0)
                                    {
                                                std::shared_ptr<mgetall_query> request = std::make_shared<mgetall_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = ++tracker;
                                                request->mmap = result_return;
                                                result.clear();
                                                request->SetOK();
                                                DataFlush::AttachResult(request);
                                      }
                                      
                                      if (aux_counter == (unsigned int)limit)
                                      {
                                                break;               
                                      }
                             }
                }
                else if (limit == -1)
                {
                             aux_counter++;
                             result_return.insert(std::make_pair(vkey, vvalue));
            
                             if (aux_counter % ITER_LIMIT == 0)
                             {
                                        std::shared_ptr<mgetall_query> request = std::make_shared<mgetall_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = ++tracker;
                                        request->mmap = result_return;
                                        result.clear();
                                        request->SetOK();
                                        DataFlush::AttachResult(request);
                             }
                }
                         
                total_counter++;
    }

     this->subresult = ++tracker;
     this->partial = false;
     this->counter = total_counter;
     this->mmap = result_return;
     this->SetOK();
}

void mgetall_query::Process()
{
        if (this->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s | %-10s", "MultiMap", "Value"));
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s | %-10s", Dispatcher::Repeat("―", 30).c_str(), Dispatcher::Repeat("―", 10).c_str()));
        }

        for (DualMMap::iterator i = this->mmap.begin(); i != this->mmap.end(); ++i)
        {
                 std::string ikey = i->first;
                 std::string item = i->second;
                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", ikey.c_str(), item.c_str()), Daemon::Format("%s %s", ikey.c_str(), item.c_str()));
        }

        if (!this->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);
        }
}

void miter_query::Run()
{
       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;

       RocksData query_result = this->Get(this->dest);

       std::shared_ptr<MultiMapHandler> handler = MultiMapHandler::Create(query_result.value);

       Args result = handler->GetVals(this->value);

       Args result_return;

       for (Args::iterator i = result.begin(); i != result.end(); ++i)
       {
                std::string hesh_as_string = (*i);

                if (this->limit != -1 && ((signed int)total_counter >= this->offset))
                {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    result_return.push_back(hesh_as_string);

                                    if (aux_counter % ITER_LIMIT == 0)
                                    {
                                                std::shared_ptr<miter_query> request = std::make_shared<miter_query>();
                                                request->user = this->user;
                                                request->partial = true;
                                                request->subresult = ++tracker;
                                                request->VecData = result_return;
                                                result.clear();
                                                request->SetOK();
                                                DataFlush::AttachResult(request);
                                      }

                                      if (aux_counter == (unsigned int)limit)
                                      {
                                                break;
                                      }
                             }
                }
                else if (limit == -1)
                {
                             aux_counter++;
                             result_return.push_back(hesh_as_string);

                             if (aux_counter % ITER_LIMIT == 0)
                             {
                                        std::shared_ptr<miter_query> request = std::make_shared<miter_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = ++tracker;
                                        request->VecData = result_return;
                                        result.clear();
                                        request->SetOK();
                                        DataFlush::AttachResult(request);
                             }
                }

                total_counter++;
    }

     this->subresult = ++tracker;
     this->partial = false;
     this->counter = total_counter;
     this->VecData = result_return;
     this->SetOK();
}

void miter_query::Process()
{
        if (this->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s", "Key"));
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s", Dispatcher::Repeat("―", 30).c_str()));
        }
        
        for (Args::iterator i = this->VecData.begin(); i != this->VecData.end(); ++i)
        {            
                 std::string item = *i;
                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s", item.c_str()), Daemon::Format("%s", item.c_str()));
        }

        if (!this->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);                 
        }
        
}
     