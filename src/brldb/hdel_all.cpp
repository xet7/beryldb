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

#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"

void hdel_all_query::Run()
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

    rocksdb::Iterator* it = this->database->db->NewIterator(rocksdb::ReadOptions());
    std::string rawstring;
    
    unsigned int total_match = 0;
    
    std::vector<std::string> rlist;
    
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
                        
                        if (asstr == this->key)
                        {
                            key_match = true;
                        }
                        else
                        {
                             break;
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
                     total_match++;                    
                     this->database->db->Delete(rocksdb::WriteOptions(), rawstring);
                }
    }    
    
    
    this->counter = total_match;
    this->SetOK();
}

void Flusher::HDelAll(User* user, std::shared_ptr<query_base> query)
{
        if (query->access == DBL_INTERRUPT || query->access == DBL_NOT_FOUND)
        {
            return;
        }

        if (query->finished)
        {
            if (query->counter > 0)
            {
                user->SendProtocol(BRLD_FLUSH, DBL_TYPE_HDEL, query->key, Daemon::Format("%d map key removed.", query->counter).c_str());
            }
            else
            {
                user->SendProtocol(BRLD_FLUSH, DBL_TYPE_HDEL, query->key, "No key founds.");
            
            }
        }
        else
        {
            user->SendProtocol(ERR_FLUSH, DBL_TYPE_HDEL, query->key, "Unable to remove key.");
        }
}
