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
#include <cmath> 

#define earth_radius_km 6371.0

#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"

/* 
 * Degrees to radians.
 * 
 * @parameters:
 *
 *         · deg: Degrees
 * 
 * @return:
 *
 *         · double: Calculation.
 */ 

double deg2rad(double deg) 
{
    return (deg * M_PI / 180);
}

/* 
 * Radians to degrees.
 * 
 * @parameters:
 *
 *         · rad: Radians.
 * 
 * @return:
 *
 *         · double: Calculation.
 */ 

double rad2deg(double rad) 
{
    return (rad * 180 / M_PI);
}

/* 
 * Calculates distance between two geopoints.
 * 
 * @parameters:
 *
 *         · lat1d: Latitude 1.
 *         · lon1d: Longitude 1.
 *           ....
 * 
 * @return:
 *
 *         · double: Calculation.
 */ 

double CalculateDistance(double lat1d, double lon1d, double lat2d, double lon2d) 
{
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    
    return 2.0 * earth_radius_km * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
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

void geocalc_query::Run()
{
        if (this->key.empty() || this->value.empty())
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
        
        std::string second = this->int_keys + this->select_query + ":" + to_bin(this->value);
        
        std::string dbvalue2;
        rocksdb::Status fstatus3 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), second, &dbvalue2);
    
        if (dbvalue2.empty())
        {
               access_set(DBL_NOT_FOUND);
               return;
        }
        
        
        size_t found =  dbvalue.find_first_of(":");
        std::string path = dbvalue.substr(0,found);
        std::string file = dbvalue.substr(found+1);
        
        size_t found2 =  dbvalue2.find_first_of(":");
        std::string path2 = dbvalue2.substr(0,found2);
        std::string file2 = dbvalue2.substr(found2+1);
        
        this->response = convto_string(CalculateDistance(convto_num<double>(to_string(path)), convto_num<double>(to_string(file)), convto_num<double>(to_string(path2)), convto_num<double>(to_string(file2))));
        this->SetOK();
}

void Flusher::GeoCalc(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
               Dispatcher::Smart(user, 1, BRLD_QUERY_OK, query->response.c_str(), query);
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
                 Dispatcher::Smart(user, 1, BRLD_ITEM, Daemon::Format("\"%s\"", key.c_str()), query);
        }

        if (!query->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);                 
        }
}

void georemove_query::Run()
{
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }
    
    std::string where = this->int_keys + this->select_query + ":" + this->key;
    std::string where_path = this->int_keys + this->select_query;

    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), this->format, &dbvalue);

    if (dbvalue.empty())
    {
         access_set(DBL_NOT_FOUND);
         return;
    }

    size_t found1 =  dbvalue.find_first_of(":");
    std::string path1 = dbvalue.substr(0,found1);
    std::string file1 = dbvalue.substr(found1+1);

    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

    Args result;

    unsigned int aux_counter = 0;

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

                /* We should not calculate our own first request. */

                if (to_string(file) == this->key)
                {
                      continue;
                }

                rawstring = rawstring.erase(0, path.length() + 1);
                rawstring = to_string(rawstring);

                std::string rawvalue = it->value().ToString();

                size_t found2 =  rawvalue.find_first_of(":");
                std::string path2 = rawvalue.substr(0,found2);
                std::string file2 = rawvalue.substr(found2+1);

                double distance = CalculateDistance(convto_num<double>(to_string(path1)), convto_num<double>(to_string(file1)), convto_num<double>(to_string(path2)), convto_num<double>(to_string(file2)));
            
                if (this->hesh == "g")
                {
                            if (distance <= convto_num<double>(this->value))
                            {
                                 aux_counter++;                
                                 this->database->GetAddress()->Delete(rocksdb::WriteOptions(), rawstring);
                            }
                }
                else
                {
                            if (distance >= convto_num<double>(this->value))
                            {
                                 aux_counter++;                
                                 this->database->GetAddress()->Delete(rocksdb::WriteOptions(), rawstring);
                            }
                }
    }

    this->counter = aux_counter;
    this->SetOK();
}

void Flusher::GeoRemove(User* user, std::shared_ptr<query_base> query)
{
        if (query->finished)
        {
                Dispatcher::Smart(user, 0, BRLD_QUERY_OK, convto_string(query->counter), query);
                return;
        }
}

void geoclose_query::Run()
{
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }

    std::string where = this->int_keys + this->select_query + ":" + this->key;
    std::string where_path = this->int_keys + this->select_query;

    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), this->format, &dbvalue);

    if (dbvalue.empty())
    {
         access_set(DBL_NOT_FOUND);
         return;
    }

    size_t found1 =  dbvalue.find_first_of(":");
    std::string path1 = dbvalue.substr(0,found1);
    std::string file1 = dbvalue.substr(found1+1);
    
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
                    
                /* We should not calculate our own first request. */
                
                if (to_string(file) == this->key)
                {
                      continue;
                }
                
                rawstring = rawstring.erase(0, path.length() + 1);
                rawstring = to_string(rawstring);
             
                std::string rawvalue = it->value().ToString();
                
                size_t found2 =  rawvalue.find_first_of(":");
                std::string path2 = rawvalue.substr(0,found2);
                std::string file2 = rawvalue.substr(found2+1);
                
                double distance = CalculateDistance(convto_num<double>(to_string(path1)), convto_num<double>(to_string(file1)), convto_num<double>(to_string(path2)), convto_num<double>(to_string(file2)));
            
                if (distance <= convto_num<double>(this->value))
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
                                                std::shared_ptr<geoclose_query> request = std::make_shared<geoclose_query>();
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
                                        std::shared_ptr<geoclose_query> request = std::make_shared<geoclose_query>();
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

void Flusher::GeoClose(User* user, std::shared_ptr<query_base> query)
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
                 Dispatcher::Smart(user, 1, BRLD_ITEM, Daemon::Format("\"%s\"", key.c_str()), query);
        }

        if (!query->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);                 
        }
}

                
