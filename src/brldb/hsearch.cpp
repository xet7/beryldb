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

/*void hsearch_query::Run()
{
    
    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

    std::string rawstring;

    unsigned int aux_counter = 0;
    unsigned int return_counter = 0;    
    unsigned int total_match = 0;
    
    unsigned int tracker = 0;
    
    std::vector<std::string> rlist;
    std::vector<std::string> aux;

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
/*
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
        if (!query->finished)
        {
             user->SendProtocol(ERR_QUERY, DBL_TYPE_HSEARCH, UNABLE_MAP);
             return;
        }

        if (query->qtype == TYPE_COUNT_RECORDS)
        {   
               user->SendProtocol(BRLD_QUERY_OK, DBL_TYPE_HSEARCH, convto_string(query->counter));
               return;
        }

        if (!query->partial && !query->counter)
        {
               Dispatcher::JustAPI(user, BRLD_START_LIST);
               Dispatcher::JustAPI(user, BRLD_END_LIST);
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

void hsearch_hesh_query::Run()
{
    
    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

    std::string rawstring;

    unsigned int total_match = 0;
    unsigned int return_counter = 0;
    
    DualMMap rlist;
    
    unsigned int tracker = 0;
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
        if (!query->finished)
        {
             user->SendProtocol(ERR_QUERY, DBL_TYPE_HSEARCH, UNABLE_MAP);
             return;
        }
        
        if (query->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);
        }
                 
        for (DualMMap::iterator i = query->mlist.begin(); i != query->mlist.end(); ++i)
        {            
                std::string key = i->first;
                std::string value = i->second;
                Dispatcher::Smart(user, 1, BRLD_ITEM, Daemon::Format("\"%s\"", key.c_str()), query);
        }
        
        if (!query->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);
        }   
                            
}
*/