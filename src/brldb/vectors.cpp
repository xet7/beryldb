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
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "brldb/vector_handler.h"
#include "brldb/dbmanager.h"
#include "helpers.h"
#include "managers/maps.h"
#include "cstruct.h"

void vsort_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}

void vsort_query::Run()
{
       RocksData result = this->Get(this->dest);

       if (!result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }

       std::shared_ptr<VectorHandler> handler = VectorHandler::Create(result.value);
       handler->Sort();
       this->Write(this->dest, handler->as_string());
       this->SetOK();
}

void vkeys_query::Run()
{
       Args result;
       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;

       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status() || this->database->IsClosing())
                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }

                std::string rawmap = it->key().ToString();
                std::string rawvalue = it->value().ToString();

                engine::colon_node_stream stream(rawmap);
                std::string token;
                unsigned int strcounter = 0;
                bool skip = false;

                std::string key_as_string;

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
                                  if (token != INT_VECTOR)
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
                                                tracker++;
                                                std::shared_ptr<vkeys_query> request = std::make_shared<vkeys_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = tracker;
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
                                        tracker++;
                                        std::shared_ptr<vkeys_query> request = std::make_shared<vkeys_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = tracker;
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

void vkeys_query::Process()
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

void vresize_query::Run()
{
       RocksData result = this->Get(this->dest);

       if (!result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }

       std::shared_ptr<VectorHandler> handler = VectorHandler::Create(result.value);

       handler->Resize(convto_num<unsigned int>(this->value));
       
       if (handler->Count() > 0)
       {
               this->Write(this->dest, handler->as_string());
       }
       else
       {
               this->Delete(this->dest);
       } 

       this->SetOK();
}

void vresize_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}

void vpush_query::Run()
{
       if (this->value.empty())
       {
             this->access_set(DBL_MISS_ARGS);
             return;
       }

       RocksData result = this->Get(this->dest);
       std::shared_ptr<VectorHandler> handler;
       
       if (!result.status.ok())
       {
               /* Create a new entry. */   
    
               handler = std::make_shared<VectorHandler>();
               handler->Add(this->value);
               
               if (handler->GetLast() == HANDLER_MSG_OK)
               {
                     this->Write(this->dest, handler->as_string());
                     this->SetOK();
               }
               else
               {
                     access_set(DBL_UNABLE_WRITE);
               }

               return;
       }
       
       handler = VectorHandler::Create(result.value);
       handler->Add(this->value);
       
       if (handler->GetLast() == HANDLER_MSG_OK)
       {
               this->Write(this->dest, handler->as_string());
               this->SetOK();
       }
       else
       {
               access_set(DBL_UNABLE_WRITE);
       }
}

void vpush_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}

void vget_query::Run()
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
       
       std::shared_ptr<VectorHandler> handler = VectorHandler::Create(query_result.value);
       
       Args result = handler->GetList();

       Args result_return;
       for (Args::iterator i = result.begin(); i != result.end(); ++i)
       {
                std::string hesh_as_string = *i;
                
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
                                                std::shared_ptr<vget_query> request = std::make_shared<vget_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = ++tracker;
                                                request->VecData = result_return;
                                                result_return.clear();
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
                                        std::shared_ptr<vget_query> request = std::make_shared<vget_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = ++tracker;
                                        request->VecData = result_return;
                                        result_return.clear();
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

void vget_query::Process()
{
        if (this->flags == QUERY_FLAGS_COUNT)
        {
               user->SendProtocol(BRLD_OK, convto_string(this->counter));
               return;
        }

        if (this->subresult == 1)
        {
               Dispatcher::JustAPI(user, BRLD_START_LIST);
        }

        for (Args::iterator i = this->VecData.begin(); i != this->VecData.end(); ++i)
        {
               std::string item = *i;
               user->SendProtocol(BRLD_ITEM, Helpers::Format(item).c_str());
        }

        if (!this->partial)
        {
               Dispatcher::JustAPI(user, BRLD_END_LIST);
        }
}

void vcount_query::Run()
{
       RocksData query_result = this->Get(this->dest);

       if (!query_result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }

       std::shared_ptr<VectorHandler> handler = VectorHandler::Create(query_result.value);
       this->counter = handler->Count();
       
       this->SetOK();
}

void vcount_query::Process()
{
       user->SendProtocol(BRLD_OK, convto_string(this->counter).c_str());
}

void vpos_query::Run()
{
       RocksData result = this->Get(this->dest);

       std::shared_ptr<VectorHandler> handler = VectorHandler::Create(result.value);
       std::string reply = handler->Index(convto_num<unsigned int>(this->value));

       if (handler->GetLast() == HANDLER_MSG_NOT_FOUND)
       {
            access_set(DBL_NOT_FOUND);
            return;
       }

       this->response = reply;
       this->SetOK();
}

void vpos_query::Process()
{
       user->SendProtocol(BRLD_OK, Helpers::Format(this->response));
}

void vexist_query::Process()
{
       user->SendProtocol(BRLD_OK, this->response.c_str());
}

void vexist_query::Run()
{
       RocksData result = this->Get(this->dest);

       if (!result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }

       std::shared_ptr<VectorHandler> handler = VectorHandler::Create(result.value);
       
       if (handler->Exist(this->value))
       {
            this->response = "1";
       }
       else
       {
            this->response = "0";
       }
       
       this->SetOK();
}

void vpop_front_query::Run()
{
       RocksData result = this->Get(this->dest);
       std::shared_ptr<VectorHandler> handler;

       if (!result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }

       handler = VectorHandler::Create(result.value);
       handler->PopFront();

       if (handler->Count() > 0)
       {
               this->Write(this->dest, handler->as_string());
       }
       else
       {
               this->Delete(this->dest);
       } 

       this->SetOK();
}

void vpop_front_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}

void vpop_back_query::Run()
{
       RocksData result = this->Get(this->dest);
       std::shared_ptr<VectorHandler> handler;

       if (!result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }

       handler = VectorHandler::Create(result.value);
       handler->PopBack();

       if (handler->Count() > 0)
       {
               this->Write(this->dest, handler->as_string());
       }
       else
       {
               this->Delete(this->dest);
       } 

       this->SetOK();
}

void vpop_back_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}

void vdel_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}

void vdel_query::Run()
{
       RocksData query_result = this->Get(this->dest);

       if (!query_result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }

       std::shared_ptr<VectorHandler> handler = VectorHandler::Create(query_result.value);
       handler->Remove(this->value);

       if (handler->Count() > 0)
       {
               this->Write(this->dest, handler->as_string());
       }
       else
       {
               this->Delete(this->dest);
       } 

       this->SetOK();
}

void verase_from_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}

void verase_from_query::Run()
{
       RocksData result = this->Get(this->dest);

       if (!result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }

       std::shared_ptr<VectorHandler> handler = VectorHandler::Create(result.value);
       handler->EraseFrom(convto_num<unsigned int>(this->value));

       if (handler->Count() > 0)
       {
               this->Write(this->dest, handler->as_string());
       }
       else
       {             
               this->Delete(this->dest);
       }
       
       this->SetOK();
}

void vreverse_query::Run()
{
       RocksData result = this->Get(this->dest);

       if (!result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }

       std::shared_ptr<VectorHandler> handler = VectorHandler::Create(result.value);
       handler->Reverse();
       this->Write(this->dest, handler->as_string());
       this->SetOK();
}

void vreverse_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}


void vrepeats_query::Run()
{
       RocksData result = this->Get(this->dest);

       if (!result.status.ok())
       {
               access_set(DBL_NOT_FOUND);
               return;
       }

       std::shared_ptr<VectorHandler> handler = VectorHandler::Create(result.value);
       this->response = convto_string(handler->Repeats(this->value));
       this->SetOK();
}

void vrepeats_query::Process()
{
       user->SendProtocol(BRLD_OK, this->response.c_str());
}

