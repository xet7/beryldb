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
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "brldb/expires.h"
#include "helpers.h"

namespace
{
       /* Thread safe random int. */
        
       unsigned int IntRand(const int & min, const int & max) 
       {
             static thread_local std::mt19937 generator;
             std::uniform_int_distribution<unsigned int> distribution(min,max);
             return distribution(generator);
        }
}

void expire_list_query::Run()
{
       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
       
       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status())
                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }

                std::string key_as_string;
                std::string select_as_string;
                
                std::string rawstring = it->key().ToString();
                std::string rawvalue = it->value().ToString();
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
                
                Kernel->Store->Expires->Add(this->database, convto_num<signed int>(rawvalue), key_as_string, select_as_string, true);
        }
}

void expire_list_query::Process()
{
        return;
}

void expire_query::Run()
{
        const std::string& wherekey = to_bin(this->key) + ":" + this->select_query + ":" + INT_KEYS;
       
        std::string dbvalue;
        rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), wherekey, &dbvalue);

        if (!fstatus2.ok())
        {
                this->access_set(DBL_NOT_FOUND);
                return;
        }
       
       this->database->GetAddress()->Put(rocksdb::WriteOptions(), this->dest, convto_string(this->id));
       Kernel->Store->Expires->Add(this->database, this->id, this->key, this->select_query, true);          

       this->SetOK();
}

void expire_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, PROCESS_OK);
}

void expireat_query::Run()
{
       const std::string& wherekey = to_bin(this->key) + ":" + this->select_query + ":" + INT_KEYS;
       
        std::string dbvalue;
        rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), wherekey, &dbvalue);

        if (!fstatus2.ok())
        {
                this->access_set(DBL_NOT_FOUND);
                return;
        }
       
       this->database->GetAddress()->Put(rocksdb::WriteOptions(), this->dest, convto_string(this->id));
       Kernel->Store->Expires->Add(this->database, this->id, this->key, this->select_query, true);          

       this->SetOK();
}

void expireat_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, PROCESS_OK);
}

void expire_del_query::Run()
{
      Kernel->Store->Expires->Delete(this->database, this->key, this->select_query);
      rocksdb::Status auxstatus = this->database->GetAddress()->Delete(rocksdb::WriteOptions(), this->dest);
      this->SetOK();
}

void expire_del_query::Process()
{
        if (!this->quiet)
        {
                user->SendProtocol(BRLD_PERSIST, this->key, PROCESS_OK);
        }
}

void set_query::Run()
{
       const std::string& where = this->dest;
       const std::string& entry_value = to_bin(this->value);
       rocksdb::Status fstatus2 =  this->database->GetAddress()->Put(rocksdb::WriteOptions(), where, entry_value);
    
       this->SetOK();
}

void set_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, PROCESS_OK);    
}

void get_query::Run()
{
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

void get_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, Helpers::Format(this->response));
}

void exists_query::Run()
{
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

void exists_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, this->response);
}

void strlen_query::Run()
{
       const std::string& where_query = this->dest;
       std::string dbvalue;
       rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where_query, &dbvalue);

       if (fstatus2.ok())
       {
                this->response = convto_string(to_string(dbvalue).length());
       }
       else
       {
               this->access_set(DBL_NOT_FOUND);
               return;
       }
       
       this->SetOK();
}

void strlen_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, this->response);
}

void count_query::Run()
{
       unsigned int total_counter = 0;

       std::string rawstring;
       std::string rawvalue;
       
       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status())
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
                                  if (token != INT_KEYS)
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

void del_query::Run()
{
        std::string dbvalue;
        rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), this->dest, &dbvalue);

        if (!fstatus2.ok())
        {
                this->access_set(DBL_NOT_FOUND);
                return;
        }

        this->database->GetAddress()->Delete(rocksdb::WriteOptions(), this->dest);
        this->DelExpire();
        this->SetOK();
}

void del_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, PROCESS_OK);
}

void getdel_query::Run()
{
       const std::string& where_query = this->dest;
       std::string dbvalue;
       rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where_query, &dbvalue);

       if (fstatus2.ok())
       {
                this->response = to_string(dbvalue);
                this->database->GetAddress()->Delete(rocksdb::WriteOptions(), this->dest);
                this->DelExpire();
                this->RemoveRegistry(this->select_query, this->base_request, this->key);
                
       }
       else
       {
               this->access_set(DBL_NOT_FOUND);
               return;
       }
       
       this->SetOK();
}

void getdel_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, Helpers::Format(this->response));
}

void getset_query::Run()
{
        if (this->value.empty())
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
               
               const std::string& where = this->dest;
               const std::string& entry_value = to_bin(this->value);
               this->database->GetAddress()->Put(rocksdb::WriteOptions(), where, entry_value);
       }
       else
       {
               this->access_set(DBL_NOT_FOUND);
               return;
       }
       
       this->SetOK();
}

void getset_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->key, Helpers::Format(this->response));
}

void find_query::Run()
{
       std::multimap<std::string, std::string> result;

       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;

       std::string rawstring;
       std::string rawvalue;
       
       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status())
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
                                  if (token != INT_KEYS)
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
                                                std::shared_ptr<find_query> request = std::make_shared<find_query>();
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
                                        std::shared_ptr<find_query> request = std::make_shared<find_query>();
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

void find_query::Process()
{
        if (this->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);
        }

        for (DualMMap::iterator i = this->mmap.begin(); i != this->mmap.end(); ++i)
        {
                 std::string key = i->first;
                 std::string value = i->second;
                 
                 Dispatcher::CondList(user, BRLD_ITEM, key, value, true); 
        }

        if (!this->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);
        }
}

void search_query::Run()
{
       Args result;
       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;

       std::string rawstring;
       
       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
       
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
                                  if (token != INT_KEYS)
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
                                                std::shared_ptr<search_query> request = std::make_shared<search_query>();
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
                                        std::shared_ptr<search_query> request = std::make_shared<search_query>();
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

void search_query::Process()
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

void rename_query::Run()
{
       std::string dbvalue;
       rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), this->dest, &dbvalue);

       if (!fstatus2.ok())
       {
               this->access_set(DBL_NOT_FOUND);
               return;
       }
       
       const std::string& newdest = to_bin(this->value) + ":" + this->select_query + ":" + this->base_request;
       
       this->database->GetAddress()->Put(rocksdb::WriteOptions(), newdest, dbvalue);

       this->SetRegistry(this->select_query, this->base_request, this->value);
       this->RemoveRegistry(this->select_query, this->base_request, this->key);
       this->database->GetAddress()->Delete(rocksdb::WriteOptions(), this->dest);
       
       /* Check if original key is expiring, and if so, copy expire settings. */
       
        signed int ttl = ExpireManager::GetTIME(this->database, this->key, this->select_query);
        
        if (ttl != -1)
        {
                const std::string& newdest = to_bin(this->value) + ":" + this->select_query + ":" + INT_EXPIRE;
                this->database->GetAddress()->Put(rocksdb::WriteOptions(), newdest, convto_string(ttl));
                Kernel->Store->Expires->Add(this->database, this->id, this->value, this->select_query, true);
                this->DelExpire();
        }
}

void rename_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}

void copy_query::Run()
{
       std::string dbvalue;
       rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), this->dest, &dbvalue);

       if (!fstatus2.ok())
       {
               this->access_set(DBL_NOT_FOUND);
               return;
       }
       
       const std::string& newdest = to_bin(this->value) + ":" + this->select_query + ":" + this->base_request;
       
       this->database->GetAddress()->Put(rocksdb::WriteOptions(), newdest, dbvalue);

       this->SetRegistry(this->select_query, this->base_request, this->value);

       /* Check if original key is expiring, and if so, copy expire settings. */
       
        signed int ttl = ExpireManager::GetTIME(this->database, this->key, this->select_query);
        
        if (ttl != -1)
        {
                const std::string& newdest = to_bin(this->value) + ":" + this->select_query + ":" + INT_EXPIRE;
                this->database->GetAddress()->Put(rocksdb::WriteOptions(), newdest, convto_string(ttl));
                Kernel->Store->Expires->Add(this->database, this->id, this->value, this->select_query, true);
        }

        this->SetOK();
}

void copy_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}

/*void append_query::Run()
{
        if (this->key.empty())
        {
                this->access_set(DBL_MISS_ARGS);
                return;
        }

        const std::string& where_query = this->format;
        
        std::string dbvalue;
        rocksdb::Status fstatus = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where_query, &dbvalue);

        if (fstatus.ok())
        {
                this->response = to_string(dbvalue);
                this->response.append(this->value);
                this->database->GetAddress()->Put(rocksdb::WriteOptions(), where_query, to_bin(this->response));
        }
        else
        {
                this->access_set(DBL_NOT_FOUND);
                return;
        }
        
        this->access_set(DBL_STATUS_OK);
        this->SetOK();
}

void Flusher::Append(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
                user->SendProtocol(BRLD_QUERY_OK, query->type, query->key, Daemon::Format("\"%s\"", query->response.c_str()).c_str());
        }
}

void advget_query::Run()
{    
      if (this->key.empty())
      {
            this->access_set(DBL_MISS_ARGS);
            return;
      }

      const std::string& where_query = this->format;
    
      std::string dbvalue;
      rocksdb::Status fstatus = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where_query, &dbvalue);

      if (fstatus.ok())
      {
            this->response = to_string(dbvalue);
      }
      else
      {
            this->access_set(DBL_NOT_FOUND);
            return;
      }

      switch (this->qtype)
      {
          case TYPE_RENAME:
          {
                const std::string& newdest = this->int_keys + this->select_query + ":" + to_bin(this->value);
                this->database->GetAddress()->Put(rocksdb::WriteOptions(), newdest, to_bin(this->response));
                this->database->GetAddress()->Delete(rocksdb::WriteOptions(), where_query);
                
                signed int ttl = ExpireManager::GetTIME(this->database, key, this->select_query);
                
                if (ttl != -1)
                {
                        Kernel->Store->Expires->Add(this->database, ttl, this->value, this->select_query, true);          
                        Kernel->Store->Expires->Delete(this->database, this->key, this->select_query);
                }

                this->SetOK();
                return;
          }

          case TYPE_RENAMENX:
          {
                const std::string& newdest = this->int_keys + this->select_query + ":" + to_bin(this->value);
                rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), newdest, &dbvalue);
                
                if (fstatus2.ok())
                {
                        this->access_set(DBL_ENTRY_EXISTS);
                        return;           
                }
                
                this->database->GetAddress()->Put(rocksdb::WriteOptions(), newdest, to_bin(this->response));
                this->database->GetAddress()->Delete(rocksdb::WriteOptions(), where_query);
                
                signed int ttl = ExpireManager::GetTIME(this->database, key, this->select_query);
                
                if (ttl != -1)
                {
                        Kernel->Store->Expires->Add(this->database, ttl, this->value, this->select_query, true);          
                        Kernel->Store->Expires->Delete(this->database, this->key, this->select_query);
                }

                this->SetOK();
                return;
          }

          case TYPE_COPY:
          {
                const std::string& newdest = this->int_keys + this->select_query + ":" + to_bin(this->value);
                this->database->GetAddress()->Put(rocksdb::WriteOptions(), newdest, to_bin(this->response));

                signed int ttl = ExpireManager::GetTIME(this->database, key, this->select_query);
                
                if (ttl != -1)
                {
                         Kernel->Store->Expires->Add(this->database, ttl, this->value, this->select_query, true);          
                }

                this->SetOK();
                return;     
          }

          case TYPE_GETDEL:
          {
                Kernel->Store->Expires->Delete(this->database, this->key, this->select_query);
                this->database->GetAddress()->Delete(rocksdb::WriteOptions(), where_query);
                this->SetOK();
                return;     
          }

          case TYPE_GETSET:
          {
                 if (this->value.empty())
                {
                        this->access_set(DBL_MISS_ARGS);
                        return;
                }
           
                this->response = to_string(dbvalue);
                this->database->GetAddress()->Put(rocksdb::WriteOptions(), where_query, to_bin(this->value));
                this->SetOK();
                return;    
          }

          case TYPE_GETRANGE:
          {
                if (from < 0 || to < 0)
                {
                        if ((long unsigned int)this->from > this->response.length())
                        {
                                  this->from = this->response.length();
                        }

                        this->response = this->response.erase(abs(this->from), abs(this->to));
                }
                else
                {
                        if ((long unsigned int)this->from > this->response.length())
                        {
                                  this->from = this->response.length();
                        }
                        
                        this->response = this->response.substr(abs(this->from), abs(this->to));
                }
                
                this->SetOK();
                return;
          }
      }
}

void Flusher::AdvancedGET(User* user, std::shared_ptr<query_base> query)
{
        switch (query->qtype)
        {
            case TYPE_GETRANGE:
            case TYPE_GETDEL:
            {
                  if (query->finished)
                  {
                        user->SendProtocol(BRLD_QUERY_OK, DBL_TYPE_GET, query->key, Daemon::Format("\"%s\"", query->response.c_str()));
                  }
                  else
                  {
                       user->SendProtocol(ERR_QUERY, DBL_TYPE_GET, query->key, PROCESS_NULL);
                  }
                
                  return;
            }
            
            case TYPE_GETSET:
            {
                if (query->finished)
                {
                        user->SendProtocol(BRLD_QUERY_OK, DBL_TYPE_GET, query->key, query->response);
                }
                else
                {
                       user->SendProtocol(ERR_QUERY, DBL_TYPE_GET, query->key, 0);
                }
                
                return;
            }
            
            case TYPE_RENAMENX:
            {
                if (query->finished)
                {
                        Dispatcher::Smart(query->user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
                }
                else
                {
                       Dispatcher::Smart(query->user, 0, ERR_QUERY, PROCESS_FALSE, query);
                }

                return;

            }
            
            case TYPE_RENAME:
            case TYPE_COPY:
            {
                if (query->finished)
                {
                        Dispatcher::Smart(query->user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
                }
                else
                {
                       Dispatcher::Smart(query->user, 0, ERR_QUERY, PROCESS_FALSE, query);
                }

                return;
            }
        }
}

void touch_query::Run()
{
        if (this->key.empty())
        {
                this->access_set(DBL_MISS_ARGS);
                return;
        }
        
        engine::space_node_stream tlist(this->key);
        std::string token;
                
        unsigned int touch_count = 0;
        unsigned int not_found = 0;
        unsigned int tcount = 0;
        unsigned int total_counter = 0;
        
        engine::space_node_stream clist(this->key);
        
        while (clist.items_extract(token))
        {
                total_counter++;
        }
        
        std::string concat;
        unsigned int iter = 0;

        while (tlist.items_extract(token))
        {	
                iter++;
                std::string tformat = this->int_keys + this->select_query + ":" + to_bin(token);

                std::string dbvalue;
                rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), tformat, &dbvalue);

                if (fstatus2.ok())
                {
                        touch_count++;
                        
                        if (this->qtype == TYPE_TCOUNT)
                        {
                                tcount += to_string(dbvalue).length();
                        }
                        else if (this->qtype == TYPE_CONCAT)
                        {
                                concat.append(to_string(dbvalue)); 
                        }
                }            
                else
                {
                        not_found++;
                } 
        }
        
        this->response = concat;
        this->id = tcount;
        this->counter = touch_count;
        this->counter2 = not_found;
        this->SetOK();
}

void Flusher::Touch(User* user, std::shared_ptr<query_base> query)
{
      if (query->finished)
      {
              switch (query->qtype)
              {
                   case TYPE_NONE:
                   
                          Dispatcher::Smart(user, query->counter, BRLD_QUERY_OK, convto_string(query->counter), query);
                          
                  return;
                   
                   case TYPE_NTOUCH:
                          
                          Dispatcher::Smart(user, query->counter2, BRLD_QUERY_OK, convto_string(query->counter2), query);
                          
                   return;
                   
                   case TYPE_TCOUNT:
                          
                          Dispatcher::Smart(user, query->id, BRLD_QUERY_OK, convto_string(query->id), query);
                          
                   return;
                   
                   case TYPE_CONCAT:
                   {
                               if (query->counter > 0)
                               {
                                      Dispatcher::Smart(user, query->counter, BRLD_QUERY_OK, Daemon::Format("\"%s\"", query->response.c_str()).c_str(), query);
                               }
                               else
                               {
                                     Dispatcher::Smart(user, query->qtype, BRLD_QUERY_OK, PROCESS_NULL, query);
                               }
                   } 
                   
                   return;
                    
                   default:
                       return;
              }
              
    }
    
    Dispatcher::Smart(query->user, 0, ERR_QUERY, UNKNOWN_ISSUE.c_str(), query);
}     

void get_query::Run()
{    
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }
    
    const std::string& where_query = this->format;

    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where_query, &dbvalue);

    if (fstatus2.ok())
    {
            this->response = to_string(dbvalue);
    }

    switch (this->qtype)
    {
            case TYPE_NONE:
            {
                if (!fstatus2.ok())
                {
                        this->access_set(DBL_NOT_FOUND);
                        return;
                }

                break;
            }

            case TYPE_LENGTH:
            {
                        if (!fstatus2.ok())
                        {
                                this->access_set(DBL_NOT_FOUND);
                                return;
                        }

                        this->length = convto_string(this->response.length());
                        break;
            }

            case TYPE_EXIST:
            case TYPE_EXPIRE:
            case TYPE_SETEX:
            case TYPE_EXPIREAT:
            {
                        if (!this->response.empty())
                        {
                                        this->exists = true;
                        } 
                        
                        if (this->qtype == TYPE_SETEX)
                        {
                                if (this->value.empty())
                                {
                                                this->access_set(DBL_MISS_ARGS);
                                                return;
                                }
                                        
                                const std::string& entry_value = to_bin(this->value);
                                this->database->GetAddress()->Put(rocksdb::WriteOptions(), where_query, entry_value);
                        }
                        
                        this->data = 0;
                        
                        if ((this->qtype == TYPE_SETEX) || (this->exists && this->qtype == TYPE_EXPIRE) || (this->exists && this->qtype == TYPE_EXPIREAT))
                        {
                                bool epoch = false;
                                
                                if (this->qtype == TYPE_EXPIREAT)
                                {
                                        epoch = true;
                                }
                                
                                signed int result = Kernel->Store->Expires->Add(this->database, this->id, this->key, this->select_query, epoch);
                                this->data = result;
                        }

                        break;
            }
    }
 
    this->SetOK();
}

void Flusher::Get(User* user, std::shared_ptr<query_base> query)
{       
        switch (query->qtype)
        {
                case TYPE_EXIST:
                {
                        Dispatcher::Smart(user, query->exists, BRLD_QUERY_OK, convto_string(query->exists), query);
                        return;       
                }

                case TYPE_LENGTH:
                {
                        if (query->finished)
                        {
                                  Dispatcher::Smart(user, 1, BRLD_QUERY_OK, query->length, query);
                        }
                        else
                        {
                                Dispatcher::Smart(user, 0, BRLD_QUERY_OK, PROCESS_NULL, query);
                        }   

                        return;
                }

                case TYPE_NONE:
                {
                        if (query->finished)
                        {
                               Dispatcher::Smart(user, 1, BRLD_QUERY_OK, Daemon::Format("\"%s\"", query->response.c_str()), query);
                        }
                        else
                        {
                              Dispatcher::Smart(user, 0, BRLD_QUERY_OK, PROCESS_NULL, query);
                        }     

                        return;      
                }

                case TYPE_EXPIREAT:
                case TYPE_EXPIRE:
                case TYPE_SETEX:
                {
                        if (query->finished)
                        {
                                        if (query->qtype == TYPE_EXPIRE || query->qtype == TYPE_SETEX || query->qtype == TYPE_EXPIREAT)
                                        {
                                                if (query->data)
                                                {
                                                        if (query->qtype == TYPE_EXPIREAT)
                                                        {
                                                                 Dispatcher::Smart(user, 1, BRLD_EXPIRE_ADD, PROCESS_OK, query);
                                                        }
                                                        else
                                                        {
                                                                Dispatcher::Smart(user, 1, BRLD_EXPIRE_ADD, PROCESS_OK, query);
                                                        }
                                                }
                                                else
                                                {
                                                        Dispatcher::Smart(user, 0, ERR_EXPIRE, PROCESS_NULL, query);
                                                }
                                                
                                        }
                                        else
                                        {
                                                Dispatcher::Smart(user, 0, ERR_EXPIRE, PROCESS_NULL, query);
                                        }    
                        } 

                        return;
                }
        }
}

void move_query::Run()
{    
        if (format.empty() || select_query.empty())
        {
                this->access_set(DBL_MISS_ARGS);
                return;
        }
        
        const std::string& current = this->format;
        const std::string& newdb = this->int_keys + this->newkey + ":" + to_bin(this->key);

        std::string dbvalue;
        rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), current, &dbvalue);

        if (!fstatus2.ok())
        {
                this->access_set(DBL_NOT_FOUND);
                return;
        }

        signed int ttl = ExpireManager::GetTIME(database, key, this->select_query);
        
        if (ttl != -1)
        {
                Kernel->Store->Expires->Delete(database, this->key, this->select_query);
                Kernel->Store->Expires->Add(database, ttl, this->key, this->newkey, true);          
        }

        this->database->GetAddress()->Delete(rocksdb::WriteOptions(), current);
        this->database->GetAddress()->Put(rocksdb::WriteOptions(), newdb, dbvalue);
        
        this->SetOK();
}

void Flusher::Move(User* user, std::shared_ptr<query_base> query)
{       
        if (query->finished)
        {
                 Dispatcher::Smart(user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
        }
}

void set_query::Run()
{    
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }
    
    const std::string& where = this->format;
    const std::string& entry_value = to_bin(this->value);
    
    switch (this->qtype)
    {
            case TYPE_SETNX:
            {
                        std::string dbvalue;
                        rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where, &dbvalue);

                        if (fstatus2.ok())
                        {
                                 this->access_set(DBL_ENTRY_EXISTS);
                                 return;
                        }
                        
                        break;      
            }

            case TYPE_SETTX:
            {
                        signed int ttl = ExpireManager::GetTIME(this->database, this->key, this->select_query);
           
                        if (ttl != -1)
                        {
                                 Kernel->Store->Expires->Add(this->database, ttl, this->value, this->select_query, true);          
                                 this->access_set(DBL_ENTRY_EXISTS);
                        }   
                        
                        break;
            }
      }

      rocksdb::Status fstatus2 =  this->database->GetAddress()->Put(rocksdb::WriteOptions(), where, entry_value);
      this->SetOK();
}

void Flusher::Set(User* user, std::shared_ptr<query_base> query)
{       
        if (query && !query->customreply.empty())
        {
               Dispatcher::Smart(user, 1, BRLD_QUERY_OK, query->customreply, query);
               return;
        }
        
        if (query->qtype == TYPE_SETNX || query->qtype == TYPE_SETTX)
        {
              Dispatcher::Smart(user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
              return;
        }
        
        if (query->finished)
        {
              Dispatcher::Smart(user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
        }
        else
        {
              Dispatcher::Smart(user, 1, BRLD_QUERY_OK, UNABLE_SET_KEY.c_str(), query);
        }
}

void del_query::Run()
{
        if (this->key.empty())
        {
                this->access_set(DBL_MISS_ARGS);
                return;
        }

        const std::string& where = this->format;

        std::string dbvalue;
        rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where, &dbvalue);

        if (!fstatus2.ok())
        {
                this->access_set(DBL_NOT_FOUND);
                return;
        }

        /* Deletes key in case it is expiring. */
        
  /*      Kernel->Store->Expires->Delete(this->database, this->key, this->select_query);
        rocksdb::Status auxstatus = this->database->GetAddress()->Delete(rocksdb::WriteOptions(), where);
        this->SetOK();
}

void Flusher::Del(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
               Dispatcher::Smart(user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
        }
}

void find_query::Run()
{
    if (this->qtype != TYPE_RAKEY && this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }
    
    std::string where = this->int_keys + this->select_query + ":" + this->key;
    std::string where_path = this->int_keys + this->select_query;
    
    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
    
    Args result;
    
    unsigned int total_counter = 0;
    unsigned int aux_counter = 0;
    unsigned int tracker = 0;
    
    std::string rawstring;
    
    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status())
                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }
                
                rawstring = it->key().ToString();
                
                size_t found =  rawstring.find_first_of(":");
                std::string path = rawstring.substr(0,found);
                std::string file = rawstring.substr(found+1);

                if (path != where_path)
                {
                    continue;
                }

                rawstring = rawstring.erase(0, path.length() + 1);
                rawstring = to_string(rawstring);

                if (this->qtype == TYPE_COUNT_RECORDS)
                {
                        /* User only needs to count items. */
                        
    /*                    if (Daemon::Match(rawstring, key))
                        {
                                aux_counter++;
                        }
                        
                        continue;
                }
                
                if (this->qtype == TYPE_RAKEY)
                {
                        aux_counter++;
                        
                        bool last = false;

                        if (!this->response.empty())
                        {
                            if ((IntRand(1, aux_counter) == aux_counter))
                            {
                                 continue;
                            }
                        }
                        else
                        {
                              this->response = rawstring;
                              last = true;
                        }
                        
                        if ((aux_counter % 100 == IntRand(0, 5)) && last == false)
                        {
                              this->response = rawstring;
                        }
                        
                        if (IntRand(1, aux_counter) == aux_counter)
                        {
                              this->response = rawstring;
                              last = true;
                        }
                        
                        if (aux_counter > 500)
                        {
                                break;
                        }
                        
                        continue;
                        
                }
                
                if (Daemon::Match(rawstring, key))
                {
                        if (limit != -1 && ((signed int)total_counter >= offset))
                        {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    
                                    result.push_back(rawstring);
             
                                    if (aux_counter % 100 == 0)
                                    {
                                                tracker++;
                                                std::shared_ptr<find_query> request = std::make_shared<find_query>();
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
                             result.push_back(rawstring);
            
                             if (aux_counter % 100 == 0)
                             {
                                        tracker++;
                                        std::shared_ptr<find_query> request = std::make_shared<find_query>();
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
    }

    if (this->qtype == TYPE_RAKEY)
    {
            if (aux_counter == 0)
            {
                    access_set(DBL_NOT_FOUND);
                    return;
            }
            
            this->counter = aux_counter;
            this->SetOK();
            return;
    }
    
    this->subresult = ++tracker;
    this->partial = false;
    this->counter = aux_counter;
    this->VecData = result;
    this->SetOK();
}

void Flusher::Find(User* user, std::shared_ptr<query_base> query)
{
        if (query->qtype == TYPE_RAKEY)
        {
             if (query->finished)
             {
                      Dispatcher::Smart(user, 1, BRLD_QUERY_OK, query->response, query);
             }
             else
             {
                      Dispatcher::Smart(user, 0, ERR_QUERY, PROCESS_NULL, query);
             }  
             
             return;
        }

        if (!query->finished)
        {
                Dispatcher::Smart(user, 0, ERR_QUERY, PROCESS_NULL, query);
                return;
        }
        
        if (query->qtype == TYPE_COUNT_RECORDS)
        {   
                user->SendProtocol(BRLD_QUERY_OK, convto_string(query->counter));
                return;
        }
                
        if (query->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);                 
        }

        for (Args::iterator i = query->VecData.begin(); i != query->VecData.end(); ++i)
        {            
                 std::string key = *i;
                 Dispatcher::Smart(user, 1, BRLD_ITEM, Daemon::Format("\"%s\"", key.c_str()), query);
        }

        if (!query->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);                 
        }
}

*/