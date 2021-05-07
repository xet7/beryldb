/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
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

void lpush_query::Run()
{
        if (!this->Check())
        {
              this->access_set(DBL_STATUS_BROKEN);
              return;
        }

        if (this->format.empty())
        {
              this->access_set(DBL_MISS_ARGS);
              return;
        }
    
        std::string id_num = convto_string(this->id);
        std::string modified = id_num;
    
        for (size_t i = 0; i < id_num.size(); i++)
        {
            modified.insert(0, id_num.size(), id_num.size());
        }
    
        std::string where_to = this->format + ":" + modified;
    
        rocksdb::Status fstatus =  this->database->db->Put(rocksdb::WriteOptions(), where_to, this->value);
    
        /* Finished. */
    
        this->counter = this->id;      
        this->SetOK();
}

void Flusher::LPush(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
            user->SendProtocol(BRLD_FLUSH, DBL_TYPE_LPUSH, query->key, query->id, "OK");
        }
        else
        {
            user->SendProtocol(ERR_FLUSH, DBL_TYPE_LPUSH, query->key, "Unable to add item to list.");
        }
}

void lmove_query::Run()
{
    if (!this->Check())
    {
            this->access_set(DBL_STATUS_BROKEN);
            return;
    }

    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }

    rocksdb::Iterator* it = this->database->db->NewIterator(rocksdb::ReadOptions());

    std::string rawstring;

    std::string foundvalue;
    
    bool found = false;

    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
                if (this->user && this->user->IsQuitting())
                {
                    access_set(DBL_NOT_FOUND);
                    return;
                }

                if (!Kernel->Store->Flusher->Status())
                {
                    access_set(DBL_INTERRUPT);
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
            this->access_set(DBL_NO_ENTRY);
            return;
    }
    
    this->database->db->Delete(rocksdb::WriteOptions(), rawstring);

    const std::string& newformat = this->int_keys + ":" + this->hesh + ":" + this->key;
    
    rocksdb::Status fstatus =  this->database->db->Put(rocksdb::WriteOptions(), newformat, foundvalue);
    
    /* Finished. */
    
    this->counter = this->id;
    this->SetOK();
}

void Flusher::LMove(User* user, std::shared_ptr<query_base> query)
{
        if (query->access == DBL_INTERRUPT || query->access == DBL_NOT_FOUND)
        {
            return;
        }

        if (!query->finished)
        {
                   user->SendProtocol(ERR_FLUSH, DBL_TYPE_LPUSH, query->key, "Item not found.");
        }
        else
        {
                  user->SendProtocol(BRLD_FLUSH, DBL_TYPE_LPUSH, query->key, "OK");
        }
}

void lpop_query::Run()
{
    if (!this->Check())
    {
            this->access_set(DBL_STATUS_BROKEN);
            return;
    }

    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }
    
    rocksdb::Iterator* it = this->database->db->NewIterator(rocksdb::ReadOptions());
    
    std::string rawstring;
    
    bool found = false;
    
    unsigned int total = 0;
    
    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
                if (this->user && this->user->IsQuitting())
                {
                    access_set(DBL_NOT_FOUND);
                    return;
                }

                if (!Kernel->Store->Flusher->Status())
                {
                    access_set(DBL_INTERRUPT);
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
                     this->database->db->Delete(rocksdb::WriteOptions(), rawstring);
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
            this->access_set(DBL_NO_ENTRY);
            return;
    }

    this->counter = total;
    this->SetOK();
}

void Flusher::LPop(User* user, std::shared_ptr<query_base> query)
{
        if (query->access == DBL_INTERRUPT || query->access == DBL_NOT_FOUND)
        {
            return;
        }

        if (query->access == DBL_NO_ENTRY)
        {
              if (query->all)
              {
                   user->SendProtocol(ERR_FLUSH, DBL_TYPE_LPOP, query->key, "Item not found.");
              }
              else
              {
                  user->SendProtocol(ERR_FLUSH, DBL_TYPE_LPOP_ALL, query->key, "Items not found.");
              }
              
              return;
        }

        if (query->finished)
        {
                if (!query->all)
                {
                    user->SendProtocol(ERR_FLUSH, DBL_TYPE_LPOP, query->counter, Daemon::Format("%d items deleted.", query->counter).c_str());
                }
                else
                {
                    user->SendProtocol(ERR_FLUSH, DBL_TYPE_LPOP, query->key, 1, "Item deleted.");
                }
        }
        else
        {
            user->SendProtocol(ERR_FLUSH, DBL_TYPE_LPOP, query->key, 0, "An error has occured.");
        }
}

void lget_query::Run()
{
    if (!this->Check())
    {
            this->access_set(DBL_STATUS_BROKEN);
            return;
    }

    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }
    
    rocksdb::Iterator* it = this->database->db->NewIterator(rocksdb::ReadOptions());
    std::vector<std::string> rlist;
    std::string rawstring;

    unsigned int total_counter = 0;
    unsigned int aux_counter = 0;
    unsigned int return_counter = 0;
    unsigned int tracker = 0;
    
    unsigned int match_count = 0;
    
    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
                if (this->user && this->user->IsQuitting())
                {
                    access_set(DBL_NOT_FOUND);
                    return;
                }

                if (!Kernel->Store->Flusher->Status())
                {
                    access_set(DBL_INTERRUPT);
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

    this->counter = return_counter;
    this->subresult = ++tracker;
    this->partial = false;
        
    this->VecData = rlist;
    this->SetOK();
    
}

void Flusher::LGet(User* user, std::shared_ptr<query_base> query)
{
        if (query->access == DBL_INTERRUPT || query->access == DBL_NOT_FOUND)
        {
            return;
        }

        if (query->qtype == TYPE_LPOS)
        {	
                 user->SendProtocol(BRLD_LPOS, query->key, query->counter,  Daemon::Format("%u", query->counter).c_str());
                 return;
        }
        
        if (query->finished && query->qtype == TYPE_EXIST)
        {
                    user->SendProtocol(BRLD_FLUSH, DBL_TYPE_LGET, query->key, convto_string(query->exists));
                    return;
        }
    
        if (query->finished && query->qtype == TYPE_COUNT_RECORDS)
        {
                user->SendProtocol(BRLD_FLUSH, DBL_TYPE_LGET, query->key, convto_string(query->counter));
                return;
        }
    
    
        if (!query->finished)
        {
                user->SendProtocol(ERR_FLUSH, DBL_TYPE_FIND, UNABLE_MAP);
                return;
        }

        if (query->subresult == 1)
        {
                user->SendProtocol(BRLD_FIND_BEGIN, query->key, "BEGIN of HSEARCH list.");
        }

        for (Args::iterator i = query->VecData.begin(); i != query->VecData.end(); ++i)
        {            
                 std::string key = *i;
                 user->SendProtocol(BRLD_FIND_ITEM, DBL_TYPE_FIND, key, Daemon::Format("\"%s\"", key.c_str()));
        }

        if (!query->partial)
        {
                user->SendProtocol(BRLD_FIND_END, query->key, Daemon::Format("END of FIND list (%i).", query->counter).c_str());
        }
}

