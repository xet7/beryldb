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
#include "brldb/expires.h"
#include "extras.h"
#include "helpers.h"

void expire_list_query::Run()
{
       unsigned int total_counter = 0;
       
       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
       
       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status() || this->database->IsClosing())
                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }

                std::string key_as_string;
                std::string select_as_string;
                std::string db_name;
                std::string rawstring = it->key().ToString();
                engine::colon_node_stream stream(rawstring);
                
                std::string token;
                unsigned int strcounter = 0;
                bool skip = false;
                
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
                             }

                             break;

                             case 1:
                             {
                                  select_as_string = token;    
                             }

                             break;
                             
                             case 3:
                             {
                                  db_name = token;
                                  
                                  if (db_name.empty() || db_name != this->database->GetName())
                                  {
                                       continue;
                                  }
                             }
                             
                             break;
                             
                             case 2:
                             {
                                   if (token != INT_EXPIRE)
                                   {
                                           skip = true;
                                   }
                             }
                             
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
                
                std::string rawvalue = it->value().ToString();

                Kernel->Store->Expires->Add(this->database, convto_num<signed int>(rawvalue), key_as_string, select_as_string, true);
                total_counter++;
        }
        
        if (total_counter > 0)
        {
             iprint((int)total_counter, "Expires loaded from %s.", this->database->GetName().c_str());
        }
}

void expire_list_query::Process()
{
        return;
}

void setex_query::Run()
{
       this->Write(this->dest, to_bin(this->value));
       this->WriteExpire(this->key, this->select_query, this->id);
       this->SetOK();
}

void setex_query::Process()
{
       NOTIFY_MODS(OnExpireAdd, (this->user, this->database->GetName(), this->key, this->select_query, this->id));
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}

void expireat_query::Run()
{
       this->WriteExpire(this->key, this->select_query, this->id);
       this->SetOK();
}

void expireat_query::Process()
{
       NOTIFY_MODS(OnExpireAdd, (this->user, this->database->GetName(), this->key, this->select_query, this->id));
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}

void expire_del_query::Run()
{	
      this->DelExpire();
      this->SetOK();
}

void expire_del_query::Process()
{
        if (this->flags != QUERY_FLAGS_QUIET)
        {
                user->SendProtocol(BRLD_PERSIST, this->key, PROCESS_OK);
        }
        
        NOTIFY_MODS(OnExpireDel, (this->user, this->database->GetName(), this->key, this->select_query));
}

void set_query::Run()
{
       this->Write(this->dest, to_bin(this->value));
       this->SetOK();
}

void set_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);    
}

void get_substr_query::Run()
{
       RocksData result = this->Get(this->dest);
       this->response = to_string(result.value);
       
       if ((unsigned int)(this->offset + this->limit) > this->response.length())
       {	
             access_set(DBL_INVALID_RANGE);
             return;
       }
       
       this->response = this->response.substr(this->offset, this->limit);
       
       this->Write(this->dest, to_bin(this->response));
       this->SetOK();
}

void get_substr_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, Helpers::Format(this->response).c_str());
}

void get_occurs_query::Run()
{
       RocksData result = this->Get(this->dest);
       this->response = to_string(result.value);
       this->response = convto_string(count_occur(this->response, this->value));
       this->SetOK();
}

void get_occurs_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->response.c_str());
}

void get_query::Run()
{
       RocksData result = this->Get(this->dest);
       this->response = to_string(result.value);
       this->SetOK();
}

void get_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, Helpers::Format(this->response));
}

void strlen_query::Run()
{
       RocksData result = this->Get(this->dest);
       this->response = convto_string(to_string(result.value).length());
       this->SetOK();
       
       this->SetOK();
}

void strlen_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->response.c_str());
}

void count_query::Run()
{
       unsigned int total_counter = 0;

       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status() || this->database->IsClosing())

                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }

                std::string rawstring = it->key().ToString();

                engine::colon_node_stream stream(rawstring);
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
                                  if (token != INT_KEY)
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
                
                total_counter++;
    }
        
    this->counter = total_counter;
    this->SetOK();
}

void count_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, convto_string(this->counter));
}

void getdel_query::Run()
{
       RocksData result = this->Get(this->dest);
       this->response = to_string(result.value);
       this->Delete(this->dest);
       this->SetOK();
}

void getdel_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, Helpers::Format(this->response));
}

void getset_query::Run()
{
       RocksData result = this->Get(this->dest);
       this->response = to_string(result.value);

       this->Write(this->dest, to_bin(this->value));
       this->SetOK();
}

void getset_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, Helpers::Format(this->response.c_str()));
}

void wdel_query::Run()
{
       unsigned int total_counter = 0;

       std::string rawstring;
       
       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status() || this->database->IsClosing())

                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }

                rawstring = it->key().ToString();
                std::string key_as_string;
                std::string token;
                bool skip = false;
                
                unsigned int strcounter = 0;
                
                engine::colon_node_stream stream(rawstring);
                
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
                                  if (token != INT_KEY)
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

                this->Delete(rawstring);
                Kernel->Store->Expires->Delete(this->database, key_as_string, this->select_query);
                this->DelExpire();
                total_counter++;
       }
       
       this->counter = total_counter;
       this->SetOK();
}

void wdel_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, convto_string(this->counter));
}

void setnx_query::Run()
{
       RocksData result = this->Get(this->dest);
       
       if (!result.status.ok())
       {
            this->Write(this->dest, to_bin(this->value));
            this->SetOK();
            return;
       }
       
       access_set(DBL_ENTRY_EXISTS);
}

void setnx_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}

void settx_query::Run()
{
       RocksData result = this->Get(this->dest);
       
       if (this->IsExpiring() == (unsigned int)0)
       {
            this->Write(this->dest, to_bin(this->value));
            this->SetOK();
            return;
       }
       
       access_set(DBL_ENTRY_EXPIRES);
}

void settx_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}

void search_query::Run()
{
       DualMMap result;

       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;

       std::string rawstring;
       std::string rawvalue;
       
       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status() || this->database->IsClosing())

                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }

                rawstring = it->key().ToString();
                rawvalue = to_string(it->value().ToString());
                         
                engine::colon_node_stream stream(rawstring);
                std::string token;
                unsigned int strcounter = 0;
                bool skip = false;

                std::string key_as_string;
                
                if (!Daemon::Match(rawvalue, this->key))
                {
                        continue;
                }

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
                                  if (token != INT_KEY)
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
                                    
                                    result.insert(std::make_pair(key_as_string, rawvalue));
             
                                    if (aux_counter % 100 == 0)
                                    {
                                                std::shared_ptr<search_query> request = std::make_shared<search_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = ++tracker;
                                                request->mmap = result;
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
                             result.insert(std::make_pair(key_as_string, rawvalue));
            
                             if (aux_counter % 100 == 0)
                             {
                                        std::shared_ptr<search_query> request = std::make_shared<search_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = ++tracker;
                                        request->mmap = result;
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
    this->mmap = result;
    this->SetOK();
}

void search_query::Process()
{
        if (this->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);
        }

        for (DualMMap::iterator i = this->mmap.begin(); i != this->mmap.end(); ++i)
        {
                 std::string ikey = i->first;
                 std::string item = i->second;
                 
                 Dispatcher::CondList(user, BRLD_ITEM, ikey, item, true); 
        }

        if (!this->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);
        }
}

void keys_query::Run()
{
       Args result;
       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;

       std::string rawstring;
       
       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
       
       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status() || this->database->IsClosing())

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
                                  if (token != INT_KEY)
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
             
                                    if (aux_counter % 100 == 0)
                                    {
                                                tracker++;
                                                std::shared_ptr<keys_query> request = std::make_shared<keys_query>();
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
            
                             if (aux_counter % 100 == 0)
                             {
                                        tracker++;
                                        std::shared_ptr<keys_query> request = std::make_shared<keys_query>();
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

void keys_query::Process()
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

void append_query::Run()
{
       RocksData result = this->Get(this->dest);
       this->response = to_string(result.value) + this->value;
       this->Write(this->dest, to_bin(this->response));
       this->SetOK();
}

void append_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, Helpers::Format(this->response).c_str());
}

