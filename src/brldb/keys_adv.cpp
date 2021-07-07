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
/*
void keys_query::Run()
{
    if (this->key.empty())
    {
                this->access_set(DBL_MISS_ARGS);
                return;
    }

    std::string where_path = this->INT_KEY + this->select_query;
    
    std::multimap<std::string, std::string> result;
    
    unsigned int aux_counter = 0;
    unsigned int tracker = 0;

    unsigned int total_counter = 0;
    
    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
                                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status() || this->database->IsClosing())

                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }
                
                std::string rawmap = it->key().ToString();

                size_t found =  rawmap.find_first_of(":");
                std::string path = rawmap.substr(0,found);
                std::string file = rawmap.substr(found+1);

                if (path != where_path)
                {
                    continue;
                }
                
                rawmap = rawmap.erase(0, path.length() + 1);
                rawmap = to_string(rawmap);
                
                std::string value_str = to_string(it->value().ToString());
                
                if (Daemon::Match(value_str, this->key))
                {
                        if (limit != -1 && ((signed int)total_counter >= offset))
                        {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    
                                    result.insert(std::make_pair(rawmap, value_str));             
                                    
                                    if (aux_counter % 100 == 0)
                                    {
                                                tracker++;
                                                std::shared_ptr<keys_query> request = std::make_shared<keys_query>();
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
                             result.insert(std::make_pair(rawmap, value_str));
            
                             if (aux_counter % 100 == 0)
                             {
                                        tracker++;
                                        std::shared_ptr<keys_query> request = std::make_shared<keys_query>();
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

void Flusher::Search(User* user, std::shared_ptr<QueryBase> query)
{
        if (!query->finished)
        {
                Dispatcher::Smart(user, 0, ERR_QUERY, PROCESS_NULL, query);
                return;
        }

        if (query->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);
        }

        for (DualMMap::iterator i = query->mmap.begin(); i != query->mmap.end(); ++i)
        {
                 std::string key = i->first;
                 std::string value = i->second;

                 user->SendProtocol(BRLD_ITEM, key, value, Daemon::Format("%s | \"%s\"", key.c_str(), value.c_str()));
        }

        if (!query->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);
        }

}*/