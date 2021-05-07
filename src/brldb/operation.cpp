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

#include <math.h>
#include <cstdlib>

#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"

void op_query::Run()
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
    
    const std::string& where = this->format;
    std::string oper = "";
    double real_oper = 0;
    
    if (q_args.size() > 0 && !q_args[0].empty())
    {
        oper = q_args[0];
        real_oper = convto_num<double>(oper);
    }

    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->db->Get(rocksdb::ReadOptions(), where, &dbvalue);
  
    if (!fstatus2.ok())
    {
          /* dbvalue not found, so we start it at 0 */

          dbvalue = "0";
    }
    else
    {
          dbvalue = to_string(dbvalue);
    }


    if (!is_number(dbvalue, true))
    {
          this->access_set(DBL_NOT_NUM);
          Kernel->Store->Flusher->opmute = false;
          return;
    }
        
    double real_value = convto_num<double>(dbvalue);

    switch (this->operation)
    {
        case OP_INCR:
            
                real_value++;
        break;
        
        case OP_DECR:
                
                real_value--;
        break;
        
        case OP_MULT:
            
            real_value = real_value * real_oper;
            
        break;
        
        case OP_AVG:

                real_value = (real_value + real_oper) / 2;
                
        break;
        
        case OP_DIV:

                real_value = double(real_value / real_oper);
        break;
        
        
        case OP_ADD:
            
                real_value += real_oper;
        break;
        
        case OP_MIN:
            
                real_value -= real_oper;
        break;
    }
    
    std::string inserting = convto_string(real_value);        
    
    std::string newvalue = to_bin(inserting);
    this->database->db->Put(rocksdb::WriteOptions(), where, newvalue);    
    this->response = inserting;
    this->SetOK();
    Kernel->Store->Flusher->opmute = false;
}

void Flusher::Operation(User* user, std::shared_ptr<query_base> query)
{       
        if (query->finished)
        {
            user->SendProtocol(BRLD_FLUSH, DBL_TYPE_OP, Daemon::Format("\"%s\"", query->response.c_str()));
        }
        else
        {
            if (query->access == DBL_NOT_NUM)
            {
                    user->SendProtocol(ERR_FLUSH, DBL_TYPE_OP, "Not numeric.");
            }
            else
            {
                    user->SendProtocol(ERR_FLUSH, DBL_TYPE_OP, "Unable to perform operation.");
            }
            
        }
}

void dbsize_query::Run()
{
    if (!this->Check())
    {
        this->access_set(DBL_STATUS_BROKEN);
        return;
    }

    rocksdb::Iterator* it = this->database->db->NewIterator(rocksdb::ReadOptions());

    std::string rawstring;
    
    double size_calc = 0;

    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
            /* We can count size from binary keys/values. */
            
            size_calc += it->key().size() + 2;
            size_calc += it->value().size() + 2;  
    }
    
    this->SetOK();
    
    float as_mb = size_calc / 1024 / 1024;
    
    if (as_mb <= 1)
    {
         this->size  = size_calc / 1024;
         this->response = "KB";
    }
    else if (as_mb > 1 && as_mb < 1024)
    {
        this->size = as_mb;
        this->response = "MB";
    }
    else if (as_mb > 1024)
    {
          this->size = size_calc / 1024 / 1024 / 1024;
          this->response = "GB";   
    }
    
    this->size = std::trunc(this->size * 10)/10;
}

void Flusher::DBSize(User* user, std::shared_ptr<query_base> query)
{       
        if (query->finished)
        {
             user->SendProtocol(BRLD_FLUSH, DBL_TYPE_DBSIZE, query->size, query->response, Daemon::Format("%s %s", convto_string(query->size).c_str(), query->response.c_str()));
        }
        else
        {
             user->SendProtocol(ERR_FLUSH, DBL_TYPE_DBSIZE, query->access, "Unable to obtain dbsize.");
        }
}


void sflush_query::Run()
{
     if (!this->Check())
     {
            this->access_set(DBL_STATUS_BROKEN);
            return;
     }

     if (this->value.empty())
     {
            this->access_set(DBL_MISS_ARGS);
            return;
     }

     unsigned int found_counter = 0;
     
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
                
                /* By definition, this should never happen. */
                
                if (rawstring.length() < 2)
                {
                    continue;
                }
                
                if (convto_string(rawstring[0]) == convto_string(INT_KEYS))
                {
                            size_t found =  rawstring.find_first_of(":");
                            std::string path = rawstring.substr(0,found);
                            std::string file = rawstring.substr(found+1);
                
                            std::string track = path.erase(0, 1);
                
                            if (track == this->value)
                            {
                                this->database->db->Delete(rocksdb::WriteOptions(), rawstring);
                                found_counter++;
                            }
                }
                else
                {
                     engine::colon_node_stream stream(rawstring);
                std::string token;

                unsigned int strcounter = 0;
                bool match = true;

                while (stream.items_extract(token))
                {
                        if (match && strcounter == 0)
                        {
                            if (convto_string(INT_KEYS) == token)
                            {
                                match = false;
                            }
                        }

                        if (match && strcounter == 1)
                        {
                            if (this->value != token)
                            {
                                match = false;
                            }
                        }

                        strcounter++;
                }
                     
                    if (match)
                    {
                            this->database->db->Delete(rocksdb::WriteOptions(), rawstring);
                            found_counter++;
                    }
                }
     }
     
     this->counter = found_counter;
     this->SetOK();	
}

void Flusher::SFlush(User* user, std::shared_ptr<query_base> query)
{
    if (query->finished)
    {
          user->SendProtocol(BRLD_SFLUSHED, query->select_query, Daemon::Format("Select %s erased. Total items: %s", query->select_query.c_str(), convto_string(query->counter).c_str()).c_str());
    }
    else
    {
         user->SendProtocol(ERR_SFLUSH, query->select_query, Daemon::Format("Unable to slufh %s.", query->select_query.c_str()).c_str());
    }
}
