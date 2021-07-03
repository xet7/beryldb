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
#include "helpers.h"

void hset_query::Run()
{
        if (this->value.empty() || this->hesh.empty())
        {
                this->access_set(DBL_MISS_ARGS);
                return;
        }

        const std::string& where_to = this->dest;
        
        rocksdb::Status fstatus2 =  this->database->GetAddress()->Put(rocksdb::WriteOptions(), where_to, this->value);

        if (!fstatus2.ok())
        {
              this->access_set(DBL_UNABLE_WRITE);
        }
        else
        {
              this->SetOK();
        }
}

void hset_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, PROCESS_OK);
}

void hexists_query::Run()
{
       if (this->hesh.empty())
       {
                this->access_set(DBL_MISS_ARGS);
                return;
       }
        
       const std::string& where_query = this->dest;
       
       std::string dbvalue;
       rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where_query, &dbvalue);

       if (fstatus2.ok())
       {
                this->response = "1";
       }
       else
       {
               this->response = "0";
       }
       
       this->SetOK();
}

void hexists_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->response);
}

void hget_query::Run()
{
        if (this->hesh.empty())
        {
                this->access_set(DBL_MISS_ARGS);
                return;
        }

       const std::string& where_query = this->dest;
       std::string dbvalue;
       rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where_query, &dbvalue);

       if (fstatus2.ok())
       {
                this->response = to_string(dbvalue);
       }
       else
       {
               this->access_set(DBL_NOT_FOUND);
               return;
       }
       
       this->SetOK();
}

void hget_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, Helpers::Format(this->response));
}

void hdel_query::Run()
{
        const std::string& where = this->dest;

        std::string dbvalue;
        rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where, &dbvalue);

        if (!fstatus2.ok())
        {
                this->access_set(DBL_NOT_FOUND);
                return;
        }

        /* Deletes key in case it is expiring. */
        
        rocksdb::Status auxstatus = this->database->GetAddress()->Delete(rocksdb::WriteOptions(), where);
        this->SetOK();
}

void hdel_query::Process()
{
        user->SendProtocol(BRLD_QUERY_OK, this->key, PROCESS_OK);
}

void hkeys_query::Run()
{
        Args result;
        rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
        std::string rawstring;
        unsigned int tracker = 0;
        unsigned int aux_counter = 0;
        unsigned int total_counter = 0;
        
        for (it->SeekToFirst(); it->Valid(); it->Next()) 
        {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status())
                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }

                rawstring = it->key().ToString();

                engine::colon_node_stream stream(rawstring);
                std::string token;

                unsigned int strcounter = 0;
                bool skip = false;
                
                std::string key_as_string;
                std::string hesh_as_string;
                
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
                                  
                                  if (key_as_string != this->key)
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
                             
                             case 3:
                             {
                                    hesh_as_string = to_string(token);
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
                                    result.push_back(hesh_as_string);
             
                                    if (aux_counter % 100 == 0)
                                    {
                                                std::shared_ptr<hkeys_query> request = std::make_shared<hkeys_query>();
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
                             result.push_back(hesh_as_string);
            
                             if (aux_counter % 100 == 0)
                             {
                                        std::shared_ptr<hkeys_query> request = std::make_shared<hkeys_query>();
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

void hkeys_query::Process()
{
        if (this->subresult == 1)
        {
               Dispatcher::JustAPI(user, BRLD_START_LIST);                 
        }

        for (Args::iterator i = this->VecData.begin(); i != this->VecData.end(); ++i)
        {            
               std::string key = *i;
               user->SendProtocol(BRLD_ITEM, key.c_str());
        }

        if (!this->partial)
        {
               Dispatcher::JustAPI(user, BRLD_END_LIST);                 
        }
}

