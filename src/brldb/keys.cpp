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

#include <random>

#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "brldb/expires.h"

namespace
{
    int IntRand(const int & min, const int & max) 
    {
         static thread_local std::mt19937 generator;
         std::uniform_int_distribution<int> distribution(min,max);
         return distribution(generator);
    }
}

void append_query::Run()
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

    const std::string& where_query = this->format;
    
    std::string dbvalue;
    rocksdb::Status fstatus = this->database->db->Get(rocksdb::ReadOptions(), where_query, &dbvalue);

    if (fstatus.ok())
    {
           this->response = to_string(dbvalue);
           this->response.append(this->value);
           this->database->db->Put(rocksdb::WriteOptions(), where_query, to_bin(this->response));
    }
    else
    {
           this->access_set(DBL_STATUS_FAILED);
           return;
    }
    
    this->access_set(DBL_STATUS_OK);
    this->SetOK();
}

void Flusher::Append(User* user, std::shared_ptr<query_base> query)
{
      if (query->finished)
      {
               user->SendProtocol(BRLD_FLUSH, query->type, query->key, query->response);
      }
      else
      {
               user->SendProtocol(ERR_FLUSH, query->key, UNDEF_KEY);
      }
}

void advget_query::Run()
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

    const std::string& where_query = this->format;
    
    std::string dbvalue;
    rocksdb::Status fstatus = this->database->db->Get(rocksdb::ReadOptions(), where_query, &dbvalue);

    if (fstatus.ok())
    {
           this->response = to_string(dbvalue);
    }
    else
    {
           this->access_set(DBL_STATUS_FAILED);
           return;
    }

    if (this->qtype == TYPE_RENAME)
    {
           const std::string& newdest = this->int_keys + this->select_query + ":" + to_bin(this->value);
           this->database->db->Put(rocksdb::WriteOptions(), newdest, to_bin(this->response));
           this->database->db->Delete(rocksdb::WriteOptions(), where_query);
           
           signed int ttl = ExpireManager::TriggerTIME(key, this->select_query);
           
           if (ttl != -1)
           {
                Kernel->Store->Expires->Add(ttl, this->value, this->select_query, true);          
                Kernel->Store->Expires->Delete(this->key, this->select_query);
           }

           this->access_set(DBL_STATUS_OK);
           this->SetOK();
    }
    
    if (this->qtype == TYPE_COPY)
    {
           const std::string& newdest = this->int_keys + this->select_query + ":" + to_bin(this->value);
           this->database->db->Put(rocksdb::WriteOptions(), newdest, to_bin(this->response));

           signed int ttl = ExpireManager::TriggerTIME(key, this->select_query);
           
           if (ttl != -1)
           {
               Kernel->Store->Expires->Add(ttl, this->value, this->select_query, true);          
           }

           this->access_set(DBL_STATUS_OK);
           this->SetOK();
    }
    
    if (this->qtype == TYPE_GETDEL)
    {
          Kernel->Store->Expires->Delete(this->key, this->select_query);
          this->database->db->Delete(rocksdb::WriteOptions(), where_query);
          this->access_set(DBL_STATUS_OK);
          this->SetOK();
          
    }
    
    if (this->qtype == TYPE_GETSET)
    {
           if (this->value.empty())
           {
                this->access_set(DBL_MISS_ARGS);
                return;
           }
           
           this->response = to_string(dbvalue);
           
           this->database->db->Put(rocksdb::WriteOptions(), where_query, to_bin(this->value));
           this->access_set(DBL_STATUS_OK);
           this->SetOK();
    }
    
    if (this->qtype == TYPE_GETRANGE)
    {
            if (from < 0 || to < 0)
            {
                 if ((long unsigned int)this->from > this->response.length())
                {
                     this->from = this->response.length();
                }

                this->response = this->response.erase(abs(this->from), abs(this->to));
            }
            else
            {
                if ((long unsigned int)this->from > this->response.length())
                {
                     this->from = this->response.length();
                }
                
                this->response = this->response.substr(abs(this->from), abs(this->to));
            }
            
        this->access_set(DBL_STATUS_OK);
        this->SetOK();
            
    }

    this->access_set(DBL_STATUS_OK);
    this->SetOK();
}

void Flusher::AdvGet(User* user, std::shared_ptr<query_base> query)
{
        if (query->qtype == TYPE_GETDEL || query->qtype == TYPE_GETRANGE)
        {
                if (query->finished)
                {
                        user->SendProtocol(BRLD_FLUSH, DBL_TYPE_GET, query->key, Daemon::Format("\"%s\"", query->response.c_str()));
                }
                else
                {
                       user->SendProtocol(ERR_FLUSH, DBL_TYPE_GET, query->key, UNDEF_KEY);
                }
                
                return;
        }

        if (query->qtype == TYPE_GETSET)
        {
                if (query->finished)
                {
                        user->SendProtocol(BRLD_FLUSH, DBL_TYPE_GET, query->key, query->response);
                }
                else
                {
                       user->SendProtocol(ERR_FLUSH, DBL_TYPE_GET, query->key, 0);
                }
                
                return;
        }
        
        if (query->qtype == TYPE_RENAME || query->qtype == TYPE_COPY)
        {
                
                if (query->finished)
                {
                        user->SendProtocol(BRLD_FLUSH, DBL_TYPE_GET, query->key, "OK");
                }
                else
                {
                       user->SendProtocol(ERR_FLUSH, DBL_TYPE_GET, query->key, Daemon::Format("Error: %d", query->access).c_str());
                }
                
                return;
        }
}

void touch_query::Run()
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
    
    engine::space_node_stream tlist(this->key);
    std::string token;
        
    unsigned int touch_count = 0;
    unsigned int not_found = 0;
    unsigned int tcount = 0;
    unsigned int total_counter = 0;
    
    engine::space_node_stream clist(this->key);
    
    while (clist.items_extract(token))
    {
        total_counter++;
    }
    
    std::string concat;
    
    unsigned int iter = 0;

    while (tlist.items_extract(token))
    {	
            iter++;
            std::string tformat = this->int_keys + this->select_query + ":" + to_bin(token);

            std::string dbvalue;
            rocksdb::Status fstatus2 = this->database->db->Get(rocksdb::ReadOptions(), tformat, &dbvalue);

            if (fstatus2.ok())
            {
                    touch_count++;
                    
                    if (this->qtype == TYPE_TCOUNT)
                    {
                          tcount += to_string(dbvalue).length();
                    }
                    else if (this->qtype == TYPE_CONCAT)
                    {
                         concat.append(to_string(dbvalue)); 
                    }
                    else if (this->qtype == TYPE_SCONCAT)
                    {
                         concat.append(to_string(dbvalue));
                         
                         if (total_counter != iter) 
                         {
                              if (this->value.empty())
                              {
                                  concat.append(" ");
                              }
                              else
                              {
                                  concat.append(this->value);
                              }
                         }
                    } 
            }            
            else
            {
                    not_found++;
            }
            
    }
    
    this->response = concat;
    this->id = tcount;
    this->counter = touch_count;
    this->counter2 = not_found;
    this->SetOK();
}

void Flusher::Touch(User* user, std::shared_ptr<query_base> query)
{
      if (query->finished)
      {
              if (query->qtype == TYPE_NTOUCH)
              {
                  user->SendProtocol(BRLD_FLUSH, TYPE_NTOUCH, query->counter2);
              }
              else if (query->qtype == TYPE_TCOUNT)
              {
                  user->SendProtocol(BRLD_FLUSH, TYPE_TCOUNT, query->id);
              }
              else if ((query->qtype == TYPE_CONCAT || query->qtype == TYPE_SCONCAT))
              {
                  if (query->counter > 0)
                  {
                      user->SendProtocol(BRLD_FLUSH, query->qtype, query->response);
                  }
                  else
                  {
                      user->SendProtocol(ERR_FLUSH, query->qtype, UNDEF_KEY);
                  }
              }
              else
              {
                  user->SendProtocol(BRLD_FLUSH, DBL_TYPE_TOUCH, query->counter);
              }
      }
}     

void get_query::Run()
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
    
    const std::string& where_query = this->format;

    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->db->Get(rocksdb::ReadOptions(), where_query, &dbvalue);

    if (fstatus2.ok())
    {
        this->response = to_string(dbvalue);
    }

    if (this->qtype == TYPE_NONE)
    {
            if (!fstatus2.ok())
            {
                this->access_set(DBL_STATUS_FAILED);
                return;
            }
    }
    
    if (this->qtype == TYPE_LENGTH)
    {
            if (!fstatus2.ok())
            {
                this->access_set(DBL_STATUS_FAILED);
                return;
            }

            this->length = convto_string(this->response.length());

    }
    else if (this->qtype == TYPE_EXIST || this->qtype == TYPE_EXPIRE || this->qtype == TYPE_SETEX || this->qtype == TYPE_EXPIREAT)
    {
        if (!this->response.empty())
        {
              this->exists = true;
        } 
        
        if (this->qtype == TYPE_SETEX)
        {
               if (this->value.empty())
               {
                         this->access_set(DBL_MISS_ARGS);
                         return;
               }
                      
               const std::string& entry_value = to_bin(this->value);
               this->database->db->Put(rocksdb::WriteOptions(), where_query, entry_value);
        }
        
        this->data = 0;
        
        if ((this->qtype == TYPE_SETEX) || (this->exists && this->qtype == TYPE_EXPIRE) || (this->exists && this->qtype == TYPE_EXPIREAT))
        {
            bool epoch = false;
            
            if (this->qtype == TYPE_EXPIREAT)
            {
                    epoch = true;
            }
            
            signed int result = Kernel->Store->Expires->Add(this->id, this->key, this->select_query, epoch);
            this->data = result;
        }
    }
    
    this->SetOK();
}

void Flusher::Get(User* user, std::shared_ptr<query_base> query)
{
        if (query->qtype == TYPE_EXIST)
        {
                user->SendProtocol(BRLD_EXISTS, DBL_TYPE_GET, query->key, query->exists);
                return;
        }

        if (query->qtype == TYPE_LENGTH)
        {
                if (query->finished)
                {
                    user->SendProtocol(BRLD_LENGTH, DBL_TYPE_GET, query->key, query->length);
                }
                else
                {
                    user->SendProtocol(ERR_EXPIRE, query->key, UNDEF_KEY);
                }
                
                return;
        }

        if (query->qtype == TYPE_NONE)
        {
                if (query->finished)
                {
                    user->SendProtocol(BRLD_FLUSH, DBL_TYPE_GET, query->key, Daemon::Format("\"%s\"", query->response.c_str()));
                }
                else
                {
                    user->SendProtocol(ERR_FLUSH, DBL_TYPE_GET, query->key, UNDEF_KEY);
                    return;
                }
        }
        
        if (query->finished && (query->qtype == TYPE_EXPIRE || query->qtype == TYPE_SETEX || query->qtype == TYPE_EXPIREAT))
        {
              if (query->qtype == TYPE_EXPIRE || query->qtype == TYPE_SETEX || query->qtype == TYPE_EXPIREAT)
              {
                   if (query->data)
                   {
                           if (query->qtype == TYPE_EXPIREAT)
                           {
                                   user->SendProtocol(BRLD_EXPIRE_ADD, query->key, Daemon::Format("%s to be deleted in %ld seconds.", query->key.c_str(), (query->id - Kernel->Now())).c_str());
                           }
                           else
                           {
                                   user->SendProtocol(BRLD_EXPIRE_ADD, query->key, Daemon::Format("%s to be deleted in %d seconds.", query->key.c_str(), query->id).c_str());
                           }
                   }
                   else
                   {
                           user->SendProtocol(ERR_EXPIRE, query->key, UNDEF_KEY);
                   }
                   
              }
              else
              {
                  user->SendProtocol(ERR_EXPIRE, query->key, UNABLE_EXPIRE);
              }
              
              return;
        }
}

void move_query::Run()
{    
    if (!this->Check())
    {
            this->access_set(DBL_STATUS_BROKEN);
            return;
    }

    if (format.empty() || select_query.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }
    
    const std::string& current = this->format;
    const std::string& newdb = this->int_keys + this->newkey + ":" + to_bin(this->key);

    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->db->Get(rocksdb::ReadOptions(), current, &dbvalue);

    if (!fstatus2.ok())
    {
          this->access_set(DBL_STATUS_FAILED);
          return;
    }

    signed int ttl = ExpireManager::TriggerTIME(key, this->select_query);
    
    if (ttl != -1)
    {
             Kernel->Store->Expires->Delete(this->key, this->select_query);
             Kernel->Store->Expires->Add(ttl, this->key, this->newkey, true);          
    }

    this->database->db->Delete(rocksdb::WriteOptions(), current);
    this->database->db->Put(rocksdb::WriteOptions(), newdb, dbvalue);
    
    this->SetOK();
}

void Flusher::Move(User* user, std::shared_ptr<query_base> query)
{       
        if (query->finished)
        {
             user->SendProtocol(BRLD_FLUSH, DBL_TYPE_SET, query->key, 1, "OK");
        }
        else
        {
             user->SendProtocol(ERR_FLUSH, DBL_TYPE_SET, query->key, 0, NOT_FOUND_KEY);
        }
}

void set_query::Run()
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
    const std::string& entry_value = to_bin(this->value);
    
    if (this->qtype == TYPE_SETNX)
    {
        std::string dbvalue;
        rocksdb::Status fstatus2 = this->database->db->Get(rocksdb::ReadOptions(), where, &dbvalue);

        if (fstatus2.ok())
        {
            this->access_set(DBL_ENTRY_EXISTS);
            return;
        }
    }
    else if (this->qtype == TYPE_SETTX)
    {
           signed int ttl = ExpireManager::TriggerTIME(this->key, this->select_query);
           
           if (ttl != -1)
           {
                 Kernel->Store->Expires->Add(ttl, this->value, this->select_query, true);          
                this->access_set(DBL_ENTRY_EXISTS);
             return;                
           }
           
    }

    rocksdb::Status fstatus2 =  this->database->db->Put(rocksdb::WriteOptions(), where, entry_value);
  
    if (!fstatus2.ok())
    {
          this->access_set(DBL_STATUS_FAILED);
          return;
    }
    
    this->SetOK();
}

void Flusher::Set(User* user, std::shared_ptr<query_base> query)
{       
        if (query && !query->customreply.empty())
        {
             user->SendProtocol(BRLD_FLUSH, DBL_TYPE_SET, query->key, query->customreply);
             return;
        }
        
        if (query->qtype == TYPE_SETNX || query->qtype == TYPE_SETTX)
        {
             if (query->access == DBL_ENTRY_EXISTS)
             {
                    user->SendProtocol(BRLD_FLUSH, DBL_TYPE_SET, query->key, 0);
             }
             else
             {
                    user->SendProtocol(ERR_FLUSH, DBL_TYPE_SET, query->key, 1);
             }
             
             return;
        }
        
        if (query->finished)
        {
            user->SendProtocol(BRLD_FLUSH, DBL_TYPE_SET, query->key, "OK");
        }
        else
        {
            user->SendProtocol(ERR_FLUSH, DBL_TYPE_SET, query->key, "Error while setting this key.");
        }
}

void del_query::Run()
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

    std::string dbvalue;
    rocksdb::Status fstatus2 = this->database->db->Get(rocksdb::ReadOptions(), where, &dbvalue);

    if (!fstatus2.ok())
    {
            this->access_set(DBL_NOT_FOUND);
            return;
    }

    /* Deletes key in case it is expiring. */
    
    Kernel->Store->Expires->Delete(this->key, this->select_query);
    
    rocksdb::Status auxstatus = this->database->db->Delete(rocksdb::WriteOptions(), where);
    this->access_set(DBL_STATUS_OK);
    
    this->SetOK();
}

void Flusher::Del(User* user, std::shared_ptr<query_base> query)
{
        /* 
         * Quiet means no result should be shown. The 'quiet' var is
         * mostly used by the expire class, but any developer may use this.
         */
           
        if (query && query->quiet)
        {
                return;
        }
        
        if (query->finished || query->access != 0)
        {
            if (query->access == DBL_NOT_FOUND)
            {
                user->SendProtocol(ERR_FLUSH, DBL_TYPE_DEL, query->key, NOT_FOUND_KEY);
            }
            else
            {
                user->SendProtocol(BRLD_FLUSH, DBL_TYPE_DEL, query->key, "Removed.");
            }
        }
        else
        {
            user->SendProtocol(ERR_FLUSH, DBL_TYPE_DEL, UNABLE_KEY);
        }
}

void find_query::Run()
{
    if (!this->Check())
    {
        this->access_set(DBL_STATUS_BROKEN);
        return;
    }

    if (this->qtype != TYPE_RAKEY && this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }

    std::string where = this->int_keys + this->select_query + ":" + this->key;
    std::string where_path = this->int_keys + this->select_query;
    
    rocksdb::Iterator* it = this->database->db->NewIterator(rocksdb::ReadOptions());
    
    Args result;
    
    unsigned int total_counter = 0;
    unsigned int aux_counter = 0;
    unsigned int tracker = 0;
    
    std::string rawstring;
    
    
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
                
                size_t found =  rawstring.find_first_of(":");
                std::string path = rawstring.substr(0,found);
                std::string file = rawstring.substr(found+1);

                if (path != where_path)
                {
                    continue;
                }

                rawstring = rawstring.erase(0, path.length() + 1);
                rawstring = to_string(rawstring);

                if (this->qtype == TYPE_COUNT_RECORDS)
                {
                        /* User only needs to count items. */
                        
                        if (Daemon::Match(rawstring, key))
                        {
                                aux_counter++;
                        }
                        
                        continue;
                }
                
                if (this->qtype == TYPE_RAKEY)
                {
                        aux_counter++;

                        if (!this->response.empty())
                        {
                            if ((IntRand(0, 10) % aux_counter) == 0)
                            {
                                 continue;
                            }
                        }
                        else
                        {
                            this->response = rawstring;
                        }
                        
                        if ((IntRand(0, 10) % aux_counter) == 0)
                        {
                                this->response = rawstring;
                        }
                        
                        continue;
                }
                
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
                                                std::shared_ptr<find_query> request = std::make_shared<find_query>();
                                                request->user = this->user;
                                                request->partial = true;                                  
                                                request->subresult = tracker;
                                                request->VecData = result;
                                                request->counter = result.size();
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
                                        std::shared_ptr<find_query> request = std::make_shared<find_query>();
                                        request->user = this->user;
                                        request->partial = true;
                                        request->subresult = tracker;
                                        request->VecData = result;
                                        request->counter = result.size();
                                        result.clear();
                                        request->SetOK();
                                        DataFlush::AttachResult(request);
                             }
                        }
                         
                        total_counter++;
                }    
    }

    if (this->qtype == TYPE_RAKEY)
    {
            this->counter = aux_counter;
            this->SetOK();
            return;
    }


    this->subresult = ++tracker;
    this->partial = false;
    this->counter = aux_counter;
    this->VecData = result;
    this->SetOK();
}

void Flusher::Find(User* user, std::shared_ptr<query_base> query)
{
        if (query->qtype == TYPE_RAKEY)
        {
             if (query->finished)
             {
                 if (query->counter > 0)
                 {
                      user->SendProtocol(BRLD_FIND_ITEM, DBL_TYPE_GET, query->response, query->response);
                 }   
                 else
                 {
                      user->SendProtocol(ERR_FLUSH, DBL_TYPE_GET, UNABLE_MAP);
                 }
             }
             else
             {
                     user->SendProtocol(ERR_FLUSH, DBL_TYPE_GET, "An error has occured.");
             } 
             
             return;
        }

        /* User disconnected or query interrupted. */
        
        if (query->access == DBL_INTERRUPT || query->access == DBL_NOT_FOUND)
        {
            return;
        }

        if (!query->finished)
        {
                user->SendProtocol(ERR_FLUSH, DBL_TYPE_FIND, UNABLE_MAP);
                return;
        }
        
        if (query->qtype == TYPE_COUNT_RECORDS)
        {   
                std::string response = "Items:";
                
                if (query->customreply != "")
                {
                    response = query->customreply;
                }
                
                user->SendProtocol(BRLD_COUNT, DBL_TYPE_FIND, query->key, Daemon::Format("%s %d", response.c_str(), query->counter).c_str());
                return;
        }
                
        if (query->subresult == 1)
        {
                user->SendProtocol(BRLD_FIND_BEGIN, query->key, "BEGIN of FIND list.");
        }

        for (Args::iterator i = query->VecData.begin(); i != query->VecData.end(); ++i)
        {            
                 std::string key = *i;
                 user->SendProtocol(BRLD_FIND_ITEM, DBL_TYPE_FIND, key, Daemon::Format("\"%s\"", key.c_str()));
        }

        if (!query->partial)
        {
                user->SendProtocol(BRLD_FIND_END, query->key, Daemon::Format("END of FIND list (%i).", query->counter).c_str());
        }
}

