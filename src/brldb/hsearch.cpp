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

void hsearch_query::Run()
{
    if (!this->Check())
    {
        this->access_set(DBL_STATUS_BROKEN);
        return;
    }
    
    rocksdb::Iterator* it = this->database->db->NewIterator(rocksdb::ReadOptions());

    std::string rawstring;

    unsigned int aux_counter = 0;
    unsigned int return_counter = 0;    
    unsigned int total_match = 0;
    
    unsigned int tracker = 0;
    
    std::vector<std::string> rlist;
    std::vector<std::string> aux;

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

                std::string asstr;
                
                while (stream.items_extract(token))
                {
                    if (strcounter == 2)
                    {
                            asstr = to_string(token);
                    
                        if (!Daemon::Match(asstr, this->key))
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
                    /* item already present. */

                    if (std::find(rlist.begin(), rlist.end(), asstr) != rlist.end()) 
                    {
                        continue;
                    }
                    
                    if (this->qtype == TYPE_COUNT_RECORDS)
                    {   
                        return_counter++;
                        continue;           
                    }
                    
                    if (((signed int)total_match >= offset))
                    {
                        
                        if (limit != -1 && ((signed int)aux_counter < limit))
                        {
                            aux_counter++;
                            rlist.push_back(asstr);
                            aux.push_back(asstr);
                            return_counter++;
                            
                            if (return_counter % 200 == 0)
                            {
                                  tracker++;
                                  std::shared_ptr<hsearch_query> request = std::make_shared<hsearch_query>();
                                  request->user = this->user;
                                  request->partial = true;
                                  request->subresult = tracker;
                                  request->VecData = aux;
                                  request->counter = aux.size();
                                  aux.clear();
                                  request->SetOK();
                                  DataFlush::AttachResult(request);
                            
                            }
                            
                            if (aux_counter == (unsigned int)limit)
                            {
                                break;               
                            }
                            
                                    
                        }
                        else if (limit == -1)
                        {
                            rlist.push_back(asstr);
                            aux.push_back(asstr);
                            
                            if (return_counter % 200 == 0)
                            {
                                  tracker++;

                                  std::shared_ptr<hsearch_query> request = std::make_shared<hsearch_query>();
                                  request->user = this->user;
                                  request->partial = true;
                                  request->subresult = tracker;
                                  request->VecData = aux;
                                  request->counter = aux.size();
                                  aux.clear();
                                  request->SetOK();
                                  DataFlush::AttachResult(request);
                            }
                            
                            return_counter++;      

                        }
                        else
                        {
                               break;
                        }
                    }
                    
                    total_match++;

                }
    }    
    
    this->subresult = ++tracker;
    this->partial = false;
    this->counter = return_counter;
    this->VecData = aux;
    this->SetOK();
}

void Flusher::HSearch(User* user, std::shared_ptr<query_base> query)
{
        /* User disconnected or query interrupted. */
        
        if (query->access == DBL_INTERRUPT || query->access == DBL_NOT_FOUND)
        {
            return;
        }
        
        if (!query->finished)
        {
             user->SendProtocol(ERR_FLUSH, DBL_TYPE_HSEARCH, UNABLE_MAP);
             return;
        }

        if (query->qtype == TYPE_COUNT_RECORDS)
        {   
               user->SendProtocol(BRLD_COUNT, DBL_TYPE_HSEARCH, query->key, Daemon::Format("%d", query->counter).c_str());
               return;
        }

        if (!query->partial && !query->counter)
        {
               user->SendProtocol(BRLD_HSEARCH_BEGIN, query->key, "BEGIN of HSEARCH list.");
               user->SendProtocol(ERR_FLUSH, DBL_TYPE_HSEARCH, UNABLE_ITEMS);
               user->SendProtocol(BRLD_HSEARCH_END, query->counter, query->key, Daemon::Format("END of HSEARCH list (%i).", query->counter).c_str());
               return;
        }
   
        if (query->subresult == 1)
        {
                user->SendProtocol(BRLD_HSEARCH_BEGIN, query->key, "BEGIN of HSEARCH list.");
        }
                 
        for (Args::iterator i = query->VecData.begin(); i != query->VecData.end(); ++i)
        {           
               std::string key = *i;
               user->SendProtocol(BRLD_HSEARCH_ITEM, DBL_TYPE_HSEARCH, key.c_str());
        }
        
        if (!query->partial)
        {
                user->SendProtocol(BRLD_HSEARCH_END, query->counter, query->key, Daemon::Format("END of HSEARCH list (%i).", query->counter).c_str());
        }        
        
            
}
