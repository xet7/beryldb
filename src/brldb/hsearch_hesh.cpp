/*
 * BerylDB - A modular database.
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

void hsearch_hesh_query::Run()
{
    if (!this->Check())
    {
        this->access_set(DBL_STATUS_BROKEN);
        return;
    }
    
    rocksdb::Iterator* it = this->database->db->NewIterator(rocksdb::ReadOptions());

    std::string rawstring;

    unsigned int total_match = 0;
    unsigned int return_counter = 0;
    
    DualMMap rlist;
    
    unsigned int tracker = 0;
    unsigned int aux_counter = 0;

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
                std::string ashesh;
                
                while (stream.items_extract(token))
                {
                    
                    if (strcounter == 2)
                    {
                        asstr = to_string(token);
                    }
                    
                    if (strcounter == 3)
                    {
                        ashesh = to_string(token);
                        
                        if (ashesh != this->hesh)
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
                    if (this->core)
                    {
                        rlist.insert(std::make_pair(asstr, to_string(it->value().ToString())));
                        total_match++;
                        continue;
                    }

                    if (((signed int)total_match >= offset))
                    {

                        if (limit != -1 && ((signed int)aux_counter < limit))
                        {
                            aux_counter++;
                    
                            rlist.insert(std::make_pair(asstr, to_string(it->value().ToString())));
                       
                            if (return_counter % 100 == 0)
                            {
                                  tracker++;

                                  std::shared_ptr<hsearch_hesh_query> request = std::make_shared<hsearch_hesh_query>();
                                  request->user = this->user;
                                  request->partial = true;
                                  request->hesh = this->hesh;

                                  request->subresult = tracker;
                                  request->mlist = rlist;
                                  request->counter = rlist.size();
                                  rlist.clear();
                                  request->SetOK();
                                  DataFlush::AttachResult(request);

                            }
                       
                       
                          return_counter++;
                }
                else if (limit == -1)
                {

                    rlist.insert(std::make_pair(asstr, to_string(it->value().ToString())));

                            if (return_counter % 100 == 0)
                            {
                              tracker++;

                                  std::shared_ptr<hsearch_hesh_query> request = std::make_shared<hsearch_hesh_query>();
                                  request->user = this->user;
                                  request->partial = true;
                                  request->hesh = this->hesh;

                                  request->subresult = tracker;
                                  request->mlist = rlist;
                                  request->counter = rlist.size();
                                  rlist.clear();
                                  request->SetOK();
                                  DataFlush::AttachResult(request);
                            }

                    return_counter++;
                
                }
                }
                total_match++;
                
            }
                
                
    }    

    this->counter = return_counter;
    this->mlist = rlist;
    this->subresult = ++tracker;
    this->partial = false;
    this->SetOK();
    
}

void Flusher::HSearch_Hesh(User* user, std::shared_ptr<query_base> query)
{
        if (query->access == DBL_INTERRUPT || query->access == DBL_NOT_FOUND)
        {
            return;
        }

        if (!query->finished)
        {
             user->SendProtocol(ERR_FLUSH, DBL_TYPE_HSEARCH, UNABLE_MAP);
             return;
        }
        
        if (query->subresult == 1)
        {
                user->SendProtocol(BRLD_HSEARCH_BEGIN, "BEGIN of HSEARCH list.");
        }
                 
        for (DualMMap::iterator i = query->mlist.begin(); i != query->mlist.end(); ++i)
        {            
                std::string key = i->first;
                std::string value = i->second;
                user->SendProtocol(BRLD_FLUSH, DBL_TYPE_HSEARCH_HESH, Daemon::Format("%s %s \"%s\"", key.c_str(), query->hesh.c_str(), value.c_str()).c_str());
        }
        
        if (!query->partial)
        {
                user->SendProtocol(BRLD_HSEARCH_END, query->counter, Daemon::Format("END of HSEARCH list (%i).", query->counter).c_str());
        }   
                            
}
