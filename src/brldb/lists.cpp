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
#include <time.h>
#include <thread>

#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "brldb/handler.h"
#include "managers/maps.h"
/*
void lpush_query::Run()
{
        if (this->format.empty())
        {
              this->access_set(DBL_MISS_ARGS);
              return;
        }
    
        std::string id_num = convto_string(this->id);
        std::string modified = id_num;
    
        for (size_t i = 0; i < id_num.size(); i++)
        {
            modified.insert(modified.begin(), id_num.size(), id_num.size());
        }
    
        std::string where_to = this->format + ":" + modified;
    
        rocksdb::Status fstatus =  this->database->GetAddress()->Put(rocksdb::WriteOptions(), where_to, this->value);
    
        /* Finished. */
    
  /*      this->counter = this->id;      
        this->SetOK();
}

void Flusher::LPush(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
                Dispatcher::Smart(user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
        }
}

void lmove_query::Run()
{
        if (this->key.empty())
        {
                this->access_set(DBL_MISS_ARGS);
                return;
        }

        rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

        std::string rawstring;

        std::string foundvalue;
        
        bool found = false;

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

                    bool int_match = false;
                    bool key_match = false;
                    bool select_match = false;

                    while (stream.items_extract(token))
                    {
                            if (it->value().ToString() != this->value)
                            {
                                break;
                            }

                            if (strcounter == 2)
                            {
                                    if (token != this->key)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        key_match = true;
                                    }
                            }

                            if (strcounter == 0)
                            {
                                    if (this->int_keys != token)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        int_match = true;
                                    }
                            }

                            if (strcounter == 1)
                            {
                                    if (this->select_query != token)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        select_match = true;
                                    }
                            }

                            strcounter++;
                    }

                    if (select_match && int_match && key_match)
                    {                 
                            found = true;
                            foundvalue = it->value().ToString();
                            break;
                    }
        }
        
        if (!found)
        {
                this->access_set(DBL_NOT_FOUND);
                return;
        }
        
        this->database->GetAddress()->Delete(rocksdb::WriteOptions(), rawstring);
        const std::string& newformat = this->int_keys + ":" + this->hesh + ":" + this->key;
        rocksdb::Status fstatus =  this->database->GetAddress()->Put(rocksdb::WriteOptions(), newformat, foundvalue);
        
        /* Finished. */
        
    /*    this->counter = this->id;
        this->SetOK();
}

void Flusher::LMove(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
               Dispatcher::Smart(user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
        }
}

void lremove_query::Run()
{
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }
    
    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

    std::string rawstring;
    unsigned int aux_counter = 0;
    

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

                    bool int_match = false;
                    bool key_match = false;
                    bool select_match = false;

                    while (stream.items_extract(token))
                    {

                            if (strcounter == 2)
                            {
                                    if (token != this->key)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        key_match = true;
                                    }
                            }

                            if (strcounter == 0)
                            {
                                    if (this->int_keys != token)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        int_match = true;
                                    }
                            }

                            if (strcounter == 1)
                            {
                                    if (this->select_query != token)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        select_match = true;
                                    }
                            }

                            strcounter++;
                    }

                    if (select_match && int_match && key_match)
                    {
                            aux_counter++;
                            this->database->GetAddress()->Delete(rocksdb::WriteOptions(), rawstring);
                    }
    }
    
    if (!aux_counter)
    {
            this->access_set(DBL_NOT_FOUND);
            return;
    }
    
    this->SetOK();
}

void Flusher::LRemove(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
               Dispatcher::Smart(user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
        }
}

void lpop_query::Run()
{
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }
    
    bool found = false;
    
    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
    std::string rawstring;
    
    unsigned int total = 0;
    
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

                bool int_match = false;
                bool key_match = false;
                bool select_match = false;
                
                while (stream.items_extract(token))
                {	
                        if (it->value().ToString() != this->value)
                        {
                            break;
                        }
                        
                        if (strcounter == 2)
                        {
                             if (token != this->key)
                             {
                                    break;
                             }
                             else
                             {
                                    key_match = true;
                             }
                        }

                        if (strcounter == 0)
                        {
                            if (this->int_keys != token)
                            {
                                  break;
                            }
                            else
                            {
                                  int_match = true;
                            }
                        }

                        if (strcounter == 1)
                        {
                            if (this->select_query != token)
                            {
                                   break;
                            }
                            else
                            {
                                select_match = true;
                            }
                        }
                        
                        strcounter++;
                }

                if (select_match && int_match && key_match)
                {
                     this->database->GetAddress()->Delete(rocksdb::WriteOptions(), rawstring);
                     found = true;
                     
                     total++;
                     
                     if (this->all)
                     {
                          break;
                     }
                }
    }    
    
    if (!found)
    {
            this->access_set(DBL_NOT_FOUND);
            return;
    }

    this->counter = total;
    this->SetOK();
}

void Flusher::LPop(User* user, std::shared_ptr<query_base> query)
{	
        if (query->finished)
        {
                if (!query->all)
                {
                      /* LPOPALL displays all items removed. */
                      
      /*                Dispatcher::Smart(user, query->counter, BRLD_QUERY_OK, convto_string(query->counter), query);
                }
                else
                {
                     /* LPOP only removes one element. */
        /*             
                     Dispatcher::Smart(user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
                }
        }
        else
        {
                Dispatcher::Smart(user, 0, ERR_QUERY, PROCESS_NULL, query);
        }
}

void lget_query::Run()
{
        if (this->key.empty())
        {
                this->access_set(DBL_MISS_ARGS);
                return;
        }
        
        rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
        std::vector<std::string> rlist;
        std::string rawstring;

        unsigned int total_counter = 0;
        unsigned int aux_counter = 0;
        unsigned int return_counter = 0;
        unsigned int tracker = 0;
        
        unsigned int match_count = 0;
        
        bool dirty = false;
        
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
                    bool match = true;
                    
                    while (stream.items_extract(token))
                    {
                            if (match && strcounter == 0)
                            {
                                   if (this->int_keys != token)
                                   {
                                       match = false;
                                       break;
                                   }
                            }
                    
                            if (match && strcounter == 2)
                            {
                                   if (to_string(token) != this->key)
                                   {
                                       match = false;
                                       break;
                                   }
                            }
                            
                            if (match && strcounter == 1)
                            {
                                   if (this->select_query != token)
                                   {
                                       match = false;
                                       break;
                                   }
                            }
                            
                            strcounter++;
                    }

                    if (match)
                    {
                    
                            match_count++;
                            
                            if (this->qtype == TYPE_EXIST)
                            {
                                  if (it->value().ToString() == this->value)
                                  { 
                                      this->exists = true;
                                      this->SetOK();
                                      return;
                                  }
                            }
                            
                            else if (this->qtype == TYPE_LPOS)
                            {
                                  if (it->value().ToString() != this->value)
                                  {
                                        continue;
                                  }
                                  else
                                  {
                                        dirty = true;
                                  }
                                
                                  if (aux_counter == this->id)
                                  {
                                      return_counter = match_count;
                                      break;
                                  }

                                  aux_counter++;
                            }
                            
                            else if (this->qtype == TYPE_COUNT_RECORDS)
                            {
                                    return_counter++;        
                            }
                            
                            else
                            { 
                                if (this->core)
                                {
                                        rlist.push_back(to_string(it->value().ToString()));
                                        return_counter++;
                                        continue;
                                }
                                
                                if (limit != -1)
                                {
                                    if (((signed int)total_counter >= offset))
                                    {
                                            if (((signed int)aux_counter < limit))
                                            {
                                                rlist.push_back(to_string(it->value().ToString()));
                                                aux_counter++;
                
                                                if (return_counter % 200 == 0)
                                                {                              
                                                        tracker++;            
                                                        std::shared_ptr<lget_query> request = std::make_shared<lget_query>();
                                                        request->user = this->user;
                                                        request->partial = true;
                                                        request->subresult = tracker;
                                                        request->VecData = rlist;
                                                        request->counter = rlist.size();
                                                        request->qtype = this->qtype;
                                                        rlist.clear();
                                                        request->SetOK();
                                                        DataFlush::AttachResult(request);
                                                }
                                                
                                                if (return_counter == (unsigned int)limit)
                                                {
                                                    break;               
                                                }
                                                
                                                return_counter++;
                                            }
                                        }
                                }          
                                else if (limit == -1)
                                {
                                                rlist.push_back(to_string(it->value().ToString()));

                                                if (return_counter % 200 == 0)
                                                {
                                                        tracker++;
                                                        std::shared_ptr<lget_query> request = std::make_shared<lget_query>();
                                                        request->user = this->user;
                                                        request->partial = true;
                                                        request->subresult = tracker;
                                                        request->qtype = this->qtype;
                                                        request->VecData = rlist;
                                                        request->counter = rlist.size();
                                                        rlist.clear();
                                                        request->SetOK();
                                                        DataFlush::AttachResult(request);
                                                }
                                                
                                                return_counter++;  
                                    }
                                    
                                    total_counter++;
                            }
                    }
        }
        
        if (this->qtype == TYPE_COUNT_RECORDS && return_counter == 0)
        {
              this->access_set(DBL_NOT_FOUND);
              return;
        }

        if (this->qtype == TYPE_LPOS && !dirty)
        {
              this->access_set(DBL_NOT_FOUND);
              return;
        }

        this->counter = return_counter;
        this->subresult = ++tracker;
        this->partial = false;
            
        this->VecData = rlist;
        this->SetOK();
}

void Flusher::LGet(User* user, std::shared_ptr<query_base> query)
{
        if (query->qtype == TYPE_LPOS)
        {	
                 Dispatcher::Smart(user, query->counter, BRLD_QUERY_OK, convto_string(query->counter), query); 
                 return;
        }
        
        if (query->finished && query->qtype == TYPE_EXIST)
        {
                 Dispatcher::Smart(user, query->exists, BRLD_QUERY_OK, convto_string(query->exists), query);
                 return;
        }
    
        if (query->finished && query->qtype == TYPE_COUNT_RECORDS)
        {
                Dispatcher::Smart(user, query->counter, BRLD_QUERY_OK, convto_string(query->counter), query);
                return;
        }
    
        if (!query->finished)
        {
                Dispatcher::Smart(user, 0, ERR_QUERY, PROCESS_NULL, query); 
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


void lsearch_query::Run()
{
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }

    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
    std::vector<std::string> rlist;
    std::vector<std::string> allitems;
    
    unsigned int aux_counter = 0;
    unsigned int return_counter = 0;
    unsigned int total_counter = 0;
    unsigned int tracker = 0;
    
    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status())
                {
                         this->access_set(DBL_INTERRUPT);
                         return;
                }

                std::string rawstring = it->key().ToString();
                
                engine::colon_node_stream stream(rawstring);
                std::string token;

                unsigned int strcounter = 0;
                bool match = true;
                
                std::string asstr;

                while (stream.items_extract(token))
                {
                        if (match && strcounter == 0)
                        {
                            if (this->int_keys != token)
                            {
                                match = false;
                                break;
                            }
                        }

                        if (match && strcounter == 2)
                        {
                             asstr = to_string(token);
                             
                             if (!Daemon::Match(asstr, this->key))
                             {
                                  match = false;
                                  break;
                             }
                        }

                        if (match && strcounter == 1)
                        {
                            if (this->select_query != token)
                            {
                                match = false;
                                break;
                            }
                        }

                        strcounter++;
                }
                
                if (match)
                {
                        if (std::find(allitems.begin(), allitems.end(), asstr) != allitems.end()) 
                        {
                              continue;
                        }
                        
                        if (limit != -1)
                        {
                                   if (((signed int)total_counter >= offset))
                                   {
                                        if (((signed int)aux_counter < limit))
                                        {
                                            rlist.push_back(asstr);
                                            allitems.push_back(asstr);
                                            aux_counter++;

                                            if (return_counter % 100 == 0)
                                            {
                                                    tracker++;
                                                    std::shared_ptr<lsearch_query> request = std::make_shared<lsearch_query>();
                                                    request->user = this->user;
                                                    request->partial = true;
                                                    request->subresult = tracker;
                                                    request->VecData = rlist;
                                                    request->counter = rlist.size();
                                                    request->qtype = this->qtype;
                                                    rlist.clear();
                                                    request->SetOK();
                                                    DataFlush::AttachResult(request);
                                            }

                                            if (return_counter == (unsigned int)limit)
                                            {
                                                break;
                                            }

                                            return_counter++;
                                        }
                                    }
                              }
                              
                              else if (limit == -1)
                              {
                                            rlist.push_back(asstr);
                                            allitems.push_back(asstr);

                                            if (return_counter % 100 == 0)
                                            {
                                                    tracker++;
                                                    std::shared_ptr<lsearch_query> request = std::make_shared<lsearch_query>();
                                                    request->user = this->user;
                                                    request->partial = true;
                                                    request->subresult = tracker;
                                                    request->qtype = this->qtype;
                                                    request->VecData = rlist;
                                                    request->counter = rlist.size();
                                                    rlist.clear();
                                                    request->SetOK();
                                                    DataFlush::AttachResult(request);
                                            }
                                            
                                            return_counter++;  
                                }

                                total_counter++;
                }
    }                

    this->counter = return_counter;
    this->subresult = ++tracker;
    this->partial = false;

    this->VecData = rlist;
    this->SetOK();

}

void Flusher::LSearch(User* user, std::shared_ptr<query_base> query)
{
        if (!query->finished)
        {
                 Dispatcher::Smart(user, 0, BRLD_QUERY_OK, PROCESS_NULL, query);
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

void lfind_query::Run()
{
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }

    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
    std::vector<std::string> rlist;

    unsigned int aux_counter = 0;
    unsigned int return_counter = 0;
    unsigned int total_counter = 0;
    unsigned int tracker = 0;

    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status())
                {
                         this->access_set(DBL_INTERRUPT);
                         return;
                }
                
                std::string rawstring = it->key().ToString();

                engine::colon_node_stream stream(rawstring);
                std::string token;

                unsigned int strcounter = 0;
                bool match = true;

                std::string asstr;
                
                while (stream.items_extract(token))
                {
                        if (match && strcounter == 0)
                        {
                            if (this->int_keys != token)
                            {
                                match = false;
                                break;
                            }
                        }

                        if (match && strcounter == 2)
                        {
                             asstr = to_string(token);

                             if (asstr != this->key)
                             {
                                  match = false;
                                  break;
                             }
                        }

                        if (match && strcounter == 1)
                        {
                            if (this->select_query != token)
                            {
                                match = false;
                                break;
                            }
                        }

                        strcounter++;
                }
                
                if (match)
                {
                           std::string rawvalue = to_string(it->value().ToString());
                            
                           if (!Daemon::Match(rawvalue, this->value))
                           {
                                   continue;
                           }
                            
                           if (limit != -1)
                           {
                                   if (((signed int)total_counter >= offset))
                                   {
                                        if (((signed int)aux_counter < limit))
                                        {
                                            rlist.push_back(rawvalue);
                                            aux_counter++;
                                            
                                            if (return_counter % 200 == 0)
                                            {
                                                    tracker++;
                                                    std::shared_ptr<lfind_query> request = std::make_shared<lfind_query>();
                                                    request->user = this->user;
                                                    request->partial = true;
                                                    request->subresult = tracker;
                                                    request->VecData = rlist;
                                                    request->counter = rlist.size();
                                                    request->qtype = this->qtype;
                                                    rlist.clear();
                                                    request->SetOK();
                                                    DataFlush::AttachResult(request);
                                            }

                                            if (return_counter == (unsigned int)limit)
                                            {
                                                break;
                                            }

                                            return_counter++;
                                        }
                                    }
                              }

                              else if (limit == -1)
                              {
                                             rlist.push_back(rawvalue);

                                            if (return_counter % 200 == 0)
                                            {
                                                    tracker++;
                                                    std::shared_ptr<lfind_query> request = std::make_shared<lfind_query>();
                                                    request->user = this->user;
                                                    request->partial = true;
                                                    request->subresult = tracker;
                                                    request->qtype = this->qtype;
                                                    request->VecData = rlist;
                                                    request->counter = rlist.size();
                                                    rlist.clear();
                                                    request->SetOK();
                                                    DataFlush::AttachResult(request);
                                            }
                                            
                                            return_counter++;  
                                }

                                total_counter++;
                }
    }
    
    this->counter = return_counter;
    this->subresult = ++tracker;
    this->partial = false;
    this->VecData = rlist;
    this->SetOK();
}

void Flusher::LFind(User* user, std::shared_ptr<query_base> query)
{
        if (!query->finished)
        {
                Dispatcher::Smart(user, query->counter, ERR_QUERY, PROCESS_NULL, query);
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