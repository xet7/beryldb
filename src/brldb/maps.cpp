/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
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

void hget_query::Run()
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

    const std::string inserting = this->format;
    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->db->Get(rocksdb::ReadOptions(), inserting, &dbvalue);

    if (!fstatus2.ok())
    {
          this->access_set(DBL_STATUS_FAILED);
          return;
    }    
   
    this->response = to_string(dbvalue);

    if (this->qtype == TYPE_EXIST)
    {	
            if (!this->response.empty())
            {
                this->exists = true;
            }
    }
    
    this->SetOK();
}

void Flusher::HGet(User* user, std::shared_ptr<query_base> query)
{
        if (query->qtype == TYPE_EXIST)
        {
             user->SendProtocol(BRLD_FLUSH, DBL_TYPE_HGET, query->key, convto_string(query->exists).c_str());
             return;
        }
        
        if (query->finished)
        {
             user->SendProtocol(BRLD_FLUSH, query->key, Daemon::Format("\"%s\"", query->response.c_str()));
        }
        else
        {
             user->SendProtocol(ERR_FLUSH, query->key, UNABLE_GET_KEY);
        }
}

void hmove_query::Run()
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

        std::string dbvalue;
        rocksdb::Status fstatus2 = this->database->db->Get(rocksdb::ReadOptions(), this->format, &dbvalue);

        if (!fstatus2.ok())
        {
            this->access_set(DBL_STATUS_FAILED);
            return;
        }    

        const std::string newfmt = this->int_keys + ":" + this->select_query + ":" + to_bin(this->key) + ":" + to_bin(this->value);
        this->database->db->Put(rocksdb::WriteOptions(), newfmt, dbvalue);
        this->database->db->Delete(rocksdb::WriteOptions(), this->format);
        this->SetOK();
}

void Flusher::HMove(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
            user->SendProtocol(BRLD_FLUSH, DBL_TYPE_HMOVE, query->key, query->value, "OK");
        }
        else
        {
            user->SendProtocol(ERR_FLUSH, DBL_TYPE_HMOVE, UNABLE_KEY);
        }
}

void hdel_query::Run()
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

        const std::string inserting = this->format;
        std::string dbvalue;
        bool dirty = false;
        
        rocksdb::Status fstatus2 = this->database->db->Get(rocksdb::ReadOptions(), inserting, &dbvalue);

        if (fstatus2.ok())
        {
                dirty = true;
                this->database->db->Delete(rocksdb::WriteOptions(), inserting);
                
        }     
        
        if (!dirty)
        {
            this->access_set(DBL_STATUS_FAILED);
            return;
        }

        this->SetOK();
}

void Flusher::HDel(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
            user->SendProtocol(BRLD_FLUSH, DBL_TYPE_HDEL, "map key removed.");
        }
        else
        {
            user->SendProtocol(ERR_FLUSH, DBL_TYPE_HDEL, UNABLE_KEY);
        }
}

void hset_query::Run()
{
        if (!this->Check())
        {
            this->access_set(DBL_STATUS_BROKEN);
            return;
        }

        if (this->value.empty() || this->format.empty())
        {
                this->access_set(DBL_MISS_ARGS);
                return;
        }

        const std::string& where_to = this->format;
        
        rocksdb::Status fstatus2 =  this->database->db->Put(rocksdb::WriteOptions(), where_to, this->value);

        if (!fstatus2.ok())
        {
            this->access_set(DBL_STATUS_FAILED);
        }
        else
        {
            this->SetOK();
        }
}

void Flusher::HSet(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
            user->SendProtocol(BRLD_FLUSH, DBL_TYPE_HSET, 1, "OK");
        }
        else
        {
            user->SendProtocol(ERR_FLUSH, DBL_TYPE_HSET, 0, "Unable to set key.");
        }
}

void hkeys_query::Run()
{
    if (!this->Check())
    {
        this->access_set(DBL_STATUS_BROKEN);
        return;
    }
    
    rocksdb::Iterator* it = this->database->db->NewIterator(rocksdb::ReadOptions());

    std::string rawstring;
    
    unsigned int return_counter = 0;
    unsigned int total_match = 0;
    unsigned int aux_counter = 0;
    unsigned int tracker = 0;
    
    Args rlist;
    Args aux;

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
                    if (strcounter == 3)
                    {
                        ashesh = to_string(token);
                    }
                    
                        if (strcounter == 2)
                        {
                            asstr = to_string(token);
                        
                            if (asstr != this->key)
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
                       if (((signed int)total_match >= offset))
                       {
                        if (limit != -1 && ((signed int)aux_counter < limit))
                        {
                            aux_counter++;
                            aux.push_back(ashesh);
                               
                            if (return_counter % 100 == 0)
                            {
                                  tracker++;
                                  std::shared_ptr<hkeys_query> request = std::make_shared<hkeys_query>();
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
                        else if (limit == -1)
                        {
                            aux.push_back(ashesh);
                           
                            if (return_counter % 100 == 0)
                            {
                                  tracker++;
                                  std::shared_ptr<hkeys_query> request = std::make_shared<hkeys_query>();
                                  request->user = this->user;
                                  request->partial = true;
                                  request->subresult = tracker;
                                  request->Run();
                                  request->VecData = aux;
                                  request->counter = aux.size();
                                  aux.clear();
                                  request->SetOK();
                                  DataFlush::AttachResult(request);
                            }
                            
                            if (return_counter == (unsigned int)limit)
                            {
                                break;
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

void Flusher::HKeys(User* user, std::shared_ptr<query_base> query)
{
        if (!query->finished)
        {
                user->SendProtocol(ERR_FLUSH, DBL_TYPE_HKEYS, UNABLE_MAP);
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
            user->SendProtocol(BRLD_HKEYS_BEGIN, DBL_TYPE_HKEYS, Daemon::Format("Begin of HKEYS list."));
        }

        for (Args::iterator i = query->VecData.begin(); i != query->VecData.end(); ++i)
        {            
               std::string key = *i;
               user->SendProtocol(BRLD_HKEYS_ITEM, DBL_TYPE_HKEYS, Daemon::Format("%s", key.c_str()));
        }

        if (!query->partial)
        {
            user->SendProtocol(BRLD_HKEYS_END, Daemon::Format("END of HKEYS list (%i).", query->counter).c_str());;
        }
}
