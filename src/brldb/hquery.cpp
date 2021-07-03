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
/*

void hquery_query::Run()
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

                if (!this->hquery->contains.empty())
                {
                             std::string kstring = to_string(it->value().ToString());

                             if (!Daemon::Match(kstring, this->hquery->contains))
                             {
                                  continue;
                             }
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
                        if (this->hquery->key.empty())
                        {
                              key_match = true;
                              continue;
                        }
                        
                        asstr = to_string(token);
                        
                        if (!Daemon::Match(asstr, this->hquery->key))
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

                            if (INT_MAP != token)
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
                            if (this->hquery->select != token)
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

                    if (std::find(rlist.begin(), rlist.end(), asstr) != rlist.end()) 
                    {
                          continue;
                    }
                    
                    if (((signed int)total_match >= this->hquery->offset))
                    {
                        
                        if (this->hquery->limit != -1 && ((signed int)aux_counter < this->hquery->limit))
                        {
                            aux_counter++;
                            rlist.push_back(asstr);
                            aux.push_back(asstr);
                            return_counter++;
                            
                            if (return_counter % 200 == 0)
                            {
                                  tracker++;
                                  std::shared_ptr<hquery_query> request = std::make_shared<hquery_query>();
                                  request->user = this->user;
                                  request->partial = true;
                                  request->subresult = tracker;
                                  request->VecData = aux;
                                  request->counter = aux.size();
                                  aux.clear();
                                  request->SetOK();
                                  DataFlush::AttachResult(request);

                            }

                            if (aux_counter == (unsigned int)this->hquery->limit)
                            {
                                break;
                            }


                        }
                        else if (this->hquery->limit == -1)
                        {
                            rlist.push_back(asstr);
                            aux.push_back(asstr);

                            if (return_counter % 200 == 0)
                            {
                                  tracker++;

                                  std::shared_ptr<hquery_query> request = std::make_shared<hquery_query>();
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

void Flusher::HQuery(User* user, std::shared_ptr<query_base> query)
{
        if (!query->finished)
        {
             user->SendProtocol(ERR_QUERY, DBL_TYPE_HSEARCH, UNABLE_MAP);
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
               Dispatcher::Smart(user, 1, BRLD_QUERY_OK, Daemon::Format("\"%s\"", key.c_str()), query);
        }
        
        if (!query->partial)
        {
               Dispatcher::JustAPI(user, BRLD_END_LIST);
        }        
}

*/