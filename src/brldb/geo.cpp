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

#include <math.h>  

#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"

double CalculateDistance(double lat1, double long1, double lat2, double long2) 
{
      double dist;
      dist = sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(long1 - long2);
      dist = acos(dist);
      dist = (6371 * M_PI * dist) / 180;
      return dist;
}

void geoadd_query::Run()
{
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }

    const std::string& where = this->format;
    const std::string& entry_value = to_bin(this->value);
    const std::string& hesh_value = to_bin(this->hesh);
    
    const std::string save = entry_value + ":" + hesh_value;
    
    rocksdb::Status fstatus2 =  this->database->GetAddress()->Put(rocksdb::WriteOptions(), where, save);
    this->SetOK();
}

void Flusher::GeoAdd(User* user, std::shared_ptr<query_base> query)
{       
        Dispatcher::Smart(user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
}


void geoget_query::Run()
{
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }

    const std::string& where_query = this->format;

    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), where_query, &dbvalue);
    
    if (dbvalue.empty())
    {
         access_set(DBL_NOT_FOUND);
         return;
    }
    
    size_t found =  dbvalue.find_first_of(":");
    std::string path = dbvalue.substr(0,found);
    std::string file = dbvalue.substr(found+1);
    
    this->response = to_string(path) + " " + to_string(file);
    this->SetOK();
}

void Flusher::GeoGet(User* user, std::shared_ptr<query_base> query)
{
      Dispatcher::Smart(user, 1, BRLD_QUERY_OK, query->response, query);
}


void geodel_query::Run()
{
        if (this->key.empty())
        {
                this->access_set(DBL_MISS_ARGS);
                return;
        }
        
        const std::string& where = this->format;
        rocksdb::Status auxstatus = this->database->GetAddress()->Delete(rocksdb::WriteOptions(), where);
        this->SetOK();
}

void Flusher::GeoDel(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
               Dispatcher::Smart(user, 1, BRLD_QUERY_OK, PROCESS_OK, query);
        }
}
        
void geofind_query::Run()
{
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }

    std::string where = this->int_keys + this->select_query + ":" + this->key;
    std::string where_path = this->int_keys + this->select_query;

    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

    Args result;

    unsigned int total_counter = 0;
    unsigned int aux_counter = 0;
    unsigned int tracker = 0;

    std::string rawstring;
    
    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status())
                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }
                
                rawstring = it->key().ToString();

                size_t found =  rawstring.find_first_of(":");
                std::string path = rawstring.substr(0,found);
                std::string file = rawstring.substr(found+1);

                if (path != where_path)
                {
                    continue;
                }

                rawstring = rawstring.erase(0, path.length() + 1);
                rawstring = to_string(rawstring);
                
                if (Daemon::Match(rawstring, key))
                {
                        if (limit != -1 && ((signed int)total_counter >= offset))
                        {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    
                                    result.push_back(rawstring);
             
                                    if (aux_counter % 200 == 0)
                                    {
                                                tracker++;
                                                std::shared_ptr<geofind_query> request = std::make_shared<geofind_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = tracker;
                                                request->VecData = result;
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
                             result.push_back(rawstring);
            
                             if (aux_counter % 200 == 0)
                             {
                                        tracker++;
                                        std::shared_ptr<geofind_query> request = std::make_shared<geofind_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = tracker;
                                        request->VecData = result;
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
    this->VecData = result;
    this->SetOK();
}

void Flusher::GeoFind(User* user, std::shared_ptr<query_base> query)
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

        for (Args::iterator i = query->VecData.begin(); i != query->VecData.end(); ++i)
        {            
                 std::string key = *i;
                 Dispatcher::Smart(user, 1, BRLD_FIND_ITEM, Daemon::Format("\"%s\"", key.c_str()), query);
        }

        if (!query->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);                 
        }
}