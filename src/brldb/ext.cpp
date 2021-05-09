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

#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "brldb/expires.h"

void search_query::Run()
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

    std::string where_path = this->int_keys + this->select_query;
    
    std::multimap<std::string, std::string> result;
    
    unsigned int aux_counter = 0;
    unsigned int tracker = 0;

    unsigned int total_counter = 0;
    
    rocksdb::Iterator* it = this->database->db->NewIterator(rocksdb::ReadOptions());

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
                
                std::string rawstring = it->key().ToString();

                size_t found =  rawstring.find_first_of(":");
                std::string path = rawstring.substr(0,found);
                std::string file = rawstring.substr(found+1);

                if (path != where_path)
                {
                    continue;
                }
                
                rawstring = rawstring.erase(0, path.length() + 1);
                rawstring = to_string(rawstring);
                
                std::string value_str = to_string(it->value().ToString());
                
                if (Daemon::Match(value_str, this->key))
                {
                        if (limit != -1 && ((signed int)total_counter >= offset))
                        {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    
                                    result.insert(std::make_pair(rawstring, value_str));             
                                    
                                    if (aux_counter % 200 == 0)
                                    {
                                                tracker++;
                                                std::shared_ptr<search_query> request = std::make_shared<search_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = tracker;
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
                             result.insert(std::make_pair(rawstring, value_str));
            
                             if (aux_counter % 200 == 0)
                             {
                                        tracker++;
                                        std::shared_ptr<search_query> request = std::make_shared<search_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = tracker;
                                        request->mmap = result;
                                        result.clear();
                                        request->SetOK();
                                        DataFlush::AttachResult(request);
                             }
                        }
                         
                        total_counter++;
                }                  
                
    }

    this->subresult = ++tracker;
    this->partial = false;
    this->counter = aux_counter;
    this->mmap = result;
    this->SetOK();
}

void Flusher::Search(User* user, std::shared_ptr<query_base> query)
{
        if (query->access == DBL_INTERRUPT || query->access == DBL_NOT_FOUND)
        {
            return;
        }

        if (!query->finished)
        {
                user->SendProtocol(ERR_FLUSH, DBL_TYPE_FIND, UNABLE_MAP);
                return;
        }

        if (query->subresult == 1)
        {
                user->SendProtocol(BRLD_SEARCH_BEGIN, query->key, "BEGIN of SEARCH list.");
        }

        for (DualMMap::iterator i = query->mmap.begin(); i != query->mmap.end(); ++i)
        {
                 std::string key = i->first;
                 std::string value = i->second;
                 user->SendProtocol(BRLD_SEARCH_ITEM, key, Daemon::Format("%s | \"%s\"", key.c_str(), value.c_str()));
        }

        if (!query->partial)
        {
                user->SendProtocol(BRLD_SEARCH_END, query->key, Daemon::Format("END of SEARCH list (%i).", query->counter).c_str());
        }

}