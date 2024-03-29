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
#include "engine.h"

#include "brldb/geo.h"

void geoaddnx_query::Run()
{
     RocksData result = this->Get(this->dest);

     if (result.status.ok())
     {
          access_set(DBL_ENTRY_EXISTS);
          return;
     }
     
     const std::string& save = to_bin(this->hesh) + ":" + to_bin(this->value);

     if (this->Write(this->dest, save))
     {
            this->SetOK();
     }
     else
     {
           access_set(DBL_UNABLE_WRITE);
     }

     this->SetOK();
}

void geoaddnx_query::Process()
{
      user->SendProtocol(BRLD_OK, PROCESS_OK);
}

void geoadd_query::Run()
{
     const std::string& save = to_bin(this->hesh) + ":" + to_bin(this->value);
     
     if (this->Write(this->dest, save))
     {
            this->SetOK();
     }
     else
     {
           access_set(DBL_UNABLE_WRITE);
     }
}

void geoadd_query::Process()
{
      user->SendProtocol(BRLD_OK, PROCESS_OK);
}

void geoadd_pub_query::Run()
{
     const std::string save = to_bin(this->value) + ":" + to_bin(this->hesh);
     
     if (this->Write(this->dest, save))
     {
          this->SetOK();
     }	
     else
     {
           access_set(DBL_UNABLE_WRITE);
     }
}

void geoadd_pub_query::Process()
{
      user->SendProtocol(BRLD_OK, PROCESS_OK);

      engine::comma_node_stream CMD(this->newkey);
      std::string server;

      while (CMD.items_extract(server))
      {
             if (!Kernel->Engine->ValidChannel(server))
             {
                  continue;
             }
             
             Channel* chan = Kernel->Channels->Find(server);
             
             if (!chan)
             {
                  continue;
             }

             ProtocolTrigger::Messages::Publish publish(ProtocolTrigger::Messages::Publish::no_replicate, user->instance, server, "GEOADD " + this->key + " " + this->value + ":" + this->hesh);
             chan->Write(Kernel->GetBRLDEvents().publish, publish);
      }
}

void geoget_query::Run()
{
      RocksData result = this->Get(this->dest);
      std::string dbvalue = result.value;
    
      size_t found =  dbvalue.find_first_of(":");
      const std::string& path = dbvalue.substr(0,found);
      const std::string& file = dbvalue.substr(found+1);
      this->response = to_string(path) + " " + to_string(file);
      this->SetOK();
}

void geoget_query::Process()
{
       user->SendProtocol(BRLD_OK, this->response);
}

void geoget_custom_query::Run()
{
      RocksData result = this->Get(this->dest);
      std::string dbvalue = result.value;

      size_t found =  dbvalue.find_first_of(":");
      
      if (this->type == QUERY_TYPE_LONG)
      {
               const std::string& path = dbvalue.substr(0,found);
               this->response = to_string(path);
      }
      else
      {
               const std::string& file = dbvalue.substr(found+1);
               this->response = to_string(file);
      }

      this->SetOK();
}

void geoget_custom_query::Process()
{
       user->SendProtocol(BRLD_OK, this->response);
}

void gkeys_query::Run()
{
       StringVector result;
       unsigned int total_counter = 0;
       unsigned int aux_counter = 0;
       unsigned int tracker = 0;

       std::string rawmap;
       
       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                if (!Dispatcher::CheckIterator(this))
                {
                       return;
                }
                
                rawmap = it->key().ToString();
                engine::colon_node_stream stream(rawmap);
                std::string token;
                std::string key_as_string;
                
                unsigned int strcounter = 0;
                bool skip = false;
                
                while (stream.items_extract(token))
                {       
                        if (skip)
                        {
                            break;
                        }
                        
                        switch (strcounter)
                        {
                             case 0:
                             {
                                  key_as_string = to_string(token);
                                
                                  if (!Daemon::Match(key_as_string, this->key))
                                  {
                                       skip = true;
                                  }
                             }
                             
                             break;
                             
                             case 1:
                             {
                                   if (convto_string(this->select_query) != token)
                                   {
                                        skip = true;
                                   }
                             }
                             
                             break;
                             
                             case 2:
                             {
                                  if (token != INT_GEO)
                                  {
                                         skip = true;	
                                  }
                             }
                             
                             break;
                             
                             default:
                             {
                                 break;   
                             }                                     
                        }
                        
                        strcounter++;
                }
                
                if (skip)
                {
                        continue;
                }
                
                if (this->limit != -1 && ((signed int)total_counter >= this->offset))
                {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    
                                    result.push_back(key_as_string);
             
                                    if (aux_counter % ITER_LIMIT == 0)
                                    {
                                                tracker++;
                                                std::shared_ptr<gkeys_query> request = std::make_shared<gkeys_query>();
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
                             result.push_back(key_as_string);
            
                             if (aux_counter % ITER_LIMIT == 0)
                             {
                                        tracker++;
                                        std::shared_ptr<gkeys_query> request = std::make_shared<gkeys_query>();
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

    this->subresult = ++tracker;
    this->partial = false;
    this->counter = aux_counter;
    this->VecData = result;
    this->SetOK();
}

void gkeys_query::Process()
{
       Dispatcher::VectorFlush(false, "Location", this);
}

void geocalc_query::Run()
{
        std::string first = to_bin(this->key) + ":" + convto_string(this->select_query) + ":" + this->base_request;

        std::string dbvalue;
        this->database->GetAddress()->Get(rocksdb::ReadOptions(), first, &dbvalue);
    
        if (dbvalue.empty())
        {
               access_set(DBL_NOT_FOUND);
               return;
        }
        
        std::string second = to_bin(this->value) + ":" + convto_string(this->select_query) + ":" + this->base_request;
        
        std::string dbvalue2;
        this->database->GetAddress()->Get(rocksdb::ReadOptions(), second, &dbvalue2);
    
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

void geocalc_query::Process()
{
       user->SendProtocol(BRLD_OK, this->response);
}

void geodistance_query::Run()
{
    std::string first = to_bin(this->key) + ":" + convto_string(this->select_query) + ":" + this->base_request;
    
    StringVector result;
    
    unsigned int total_counter = 0;
    unsigned int aux_counter = 0;
    unsigned int tracker = 0;
    std::string rawmap;
    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), first, &dbvalue);

    if (dbvalue.empty())
    {
          access_set(DBL_NOT_FOUND);
          return;
    }
    
    size_t found1 =  dbvalue.find_first_of(":");
    std::string path1 = dbvalue.substr(0,found1);
    std::string file1 = dbvalue.substr(found1+1);
    
    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
                if (!Dispatcher::CheckIterator(this))
                {
                       return;
                }
                
                rawmap = it->key().ToString();
                engine::colon_node_stream stream(rawmap);
                std::string token;
                std::string key_as_string;

                unsigned int strcounter = 0;
                bool skip = false;

                while (stream.items_extract(token))
                {
                        if (skip)
                        {
                            break;
                        }
                        
                        switch (strcounter)
                        {
                             case 0:
                             {
                                  key_as_string = to_string(token);

                                  if (key_as_string == this->key)
                                  {
                                       skip = true;
                                  }
                             }

                             break;

                             case 1:
                             {
                                   if (convto_string(this->select_query) != token)
                                   {
                                        skip = true;
                                   }
                             }

                             break;

                             case 2:
                             {
                                  if (token != INT_GEO)
                                  {
                                         skip = true;   
                                  }
                             }
                             
                             break;

                             default:
                             {
                                 break;   
                             }
                        }

                        strcounter++;
                }

                if (skip)
                {
                        continue;
                }

                std::string rawvalue = it->value().ToString();
                
                size_t found2 =  rawvalue.find_first_of(":");
                std::string path2 = rawvalue.substr(0,found2);
                std::string file2 = rawvalue.substr(found2+1);

                double distance = CalculateDistance(convto_num<double>(to_string(path1)), convto_num<double>(to_string(file1)), convto_num<double>(to_string(path2)), convto_num<double>(to_string(file2)));
            
                if (distance <= convto_num<double>(this->value))
                {
                      continue;
                }
                
                if (this->limit != -1 && ((signed int)total_counter >= this->offset))
                {
                             if (((signed int)aux_counter < limit))
                             {
                                    aux_counter++;
                                    
                                    result.push_back(key_as_string);
             
                                    if (aux_counter % ITER_LIMIT == 0)
                                    {
                                                tracker++;
                                                std::shared_ptr<geodistance_query> request = std::make_shared<geodistance_query>();
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
                             result.push_back(key_as_string);
            
                             if (aux_counter % ITER_LIMIT == 0)
                             {
                                        tracker++;
                                        std::shared_ptr<geodistance_query> request = std::make_shared<geodistance_query>();
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

    this->subresult = ++tracker;
    this->partial = false;
    this->counter = aux_counter;
    this->VecData = result;
    this->SetOK();
}

void geodistance_query::Process()
{
        if (this->subresult == 1)
        {
                Dispatcher::JustAPI(user, BRLD_START_LIST);
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s", "Location"));
                Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s", Dispatcher::Repeat("―", 30).c_str()));
        }
        
        for (StringVector::iterator i = this->VecData.begin(); i != this->VecData.end(); ++i)
        {            
                 std::string item = *i;
                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s", item.c_str()), Daemon::Format("%s", item.c_str()));
        }

        if (!this->partial)
        {
                Dispatcher::JustAPI(user, BRLD_END_LIST);                 
        }
}

void georem_query::Run()
{
    std::string first = to_bin(this->key) + ":" + convto_string(this->select_query) + ":" + this->base_request;
    
    StringVector result;
    
    unsigned int total_counter = 0;

    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), first, &dbvalue);

    if (dbvalue.empty())
    {
          access_set(DBL_NOT_FOUND);
          return;
    }
    
    size_t found1 =  dbvalue.find_first_of(":");
    std::string path1 = dbvalue.substr(0,found1);
    std::string file1 = dbvalue.substr(found1+1);
    
    rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());

    for (it->SeekToFirst(); it->Valid(); it->Next()) 
    {
                if (!Dispatcher::CheckIterator(this))
                {
                       return;
                }
                
                std::string rawmap = it->key().ToString();
                engine::colon_node_stream stream(rawmap);
                std::string token;
                std::string key_as_string;

                unsigned int strcounter = 0;
                bool skip = false;

                while (stream.items_extract(token))
                {
                        if (skip)
                        {
                            break;
                        }
                        
                        switch (strcounter)
                        {
                             case 0:
                             {
                                  key_as_string = to_string(token);
                                  
                                  if (key_as_string == this->key)
                                  {
                                       continue;
                                  }
                             }

                             break;

                             case 1:
                             {
                                   if (convto_string(this->select_query) != token)
                                   {
                                        skip = true;
                                   }
                             }

                             break;

                             case 2:
                             {
                                  if (token != INT_GEO)
                                  {
                                         skip = true;   
                                  }
                             }
                             
                             break;

                             default:
                             {
                                 break;   
                             }
                        }

                        strcounter++;
                }

                if (skip)
                {
                        continue;
                }

                std::string rawvalue = it->value().ToString();
     
                size_t found2 =  rawvalue.find_first_of(":");
                std::string path2 = rawvalue.substr(0,found2);
                std::string file2 = rawvalue.substr(found2+1);

                double distance = CalculateDistance(convto_num<double>(to_string(path1)), convto_num<double>(to_string(file1)), convto_num<double>(to_string(path2)), convto_num<double>(to_string(file2)));
                
                if (distance >= convto_num<double>(this->value))
                {
                      continue;
                }

                this->Delete(rawmap);
                total_counter++;
    }

    this->counter = total_counter;
    this->SetOK();
}

void georem_query::Process()
{
        user->SendProtocol(BRLD_OK, convto_string(this->counter));
}



