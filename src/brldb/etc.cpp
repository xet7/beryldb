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
#include "brldb/expires.h"
#include "helpers.h"

void future_list_query::Run()
{
       unsigned int total_counter = 0;
       
       rocksdb::Iterator* it = this->database->GetAddress()->NewIterator(rocksdb::ReadOptions());
       
       for (it->SeekToFirst(); it->Valid(); it->Next()) 
       {
                if ((this->user && this->user->IsQuitting()) || !Kernel->Store->Flusher->Status() || this->database->IsClosing())
                {
                      this->access_set(DBL_INTERRUPT);
                      return;
                }

                std::string key_as_string;
                std::string select_as_string;
                std::string db_name;
                std::string rawmap = it->key().ToString();
                engine::colon_node_stream stream(rawmap);
                
                std::string token;
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
                             }

                             break;

                             case 1:
                             {
                                  select_as_string = token;    
                             }

                             break;
                             
                             case 3:
                             {
                                  db_name = token;
                                  
                                  if (db_name.empty() || db_name != this->database->GetName())
                                  {
                                       continue;
                                  }
                             }
                             
                             break;
                             
                             case 2:
                             {
                                   if (token != INT_FUTURE)
                                   {
                                           skip = true;
                                   }
                             }
                             
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
                
                if (Kernel->Config->KeepFutures)
                {
                     Kernel->Store->Futures->Add(this->database, convto_num<signed int>(path2), key_as_string, file2, select_as_string, true);
                     total_counter++;
                }
                else
                {
                     this->Delete(rawmap);
                }
        }
        
        if (Kernel->Config->KeepFutures && total_counter > 0)
        {
             iprint((int)total_counter, "Futures loaded from %s.", this->database->GetName().c_str());
        }
}

void future_list_query::Process()
{
        return;
}

void future_query::Run()
{
       this->WriteFuture(this->key, this->select_query, this->id, this->value);
       this->SetOK();
}

void future_query::Process()
{
       NOTIFY_MODS(OnFutureAdd, (this->user, this->database->GetName(), this->key, this->select_query));
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}

void expire_query::Run()
{
       this->WriteExpire(this->key, this->select_query, this->id);
       this->SetOK();
}

void expire_query::Process()
{
       NOTIFY_MODS(OnExpireAdd, (this->user, this->database->GetName(), this->key, this->select_query, this->id));
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}

void future_exec_query::Run()
{
      std::string lookup = to_bin(this->key) + ":" + this->select_query + ":" + INT_FUTURE + ":" + this->database->GetName();
      std::string dbvalue;
      rocksdb::Status fstatus = this->database->GetAddress()->Get(rocksdb::ReadOptions(), lookup, &dbvalue);       
      
      if (fstatus.ok())
      {
             size_t found2 =  dbvalue.find_first_of(":");
             std::string path2 = dbvalue.substr(0,found2);
             std::string file2 = dbvalue.substr(found2+1);
             
             this->SetDest(this->key, this->select_query, INT_KEY);
             this->Write(this->dest, to_bin(file2));
             this->DelFuture();
             this->SetOK();
             return;
       }
       
       this->DelFuture();
       access_set(DBL_NOT_FOUND);
}

void future_exec_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}

void future_del_query::Run()
{
      std::string lookup = to_bin(this->key) + ":" + this->select_query + ":" + INT_FUTURE + ":" + this->database->GetName();
      std::string dbvalue;
      rocksdb::Status fstatus = this->database->GetAddress()->Get(rocksdb::ReadOptions(), lookup, &dbvalue);       
      
      if (fstatus.ok())
      {
             this->DelFuture();
             this->SetOK();
             return;
       }
       
       access_set(DBL_NOT_FOUND);
}

void future_del_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}

void dbreset_query::Run()
{
      database->SetClosing(true);

      ExpireManager::PreDBClose(this->key);
      FutureManager::PreDBClose(this->key);
}

void dbreset_query::Process()
{
      std::shared_ptr<UserDatabase> userdb = Kernel->Store->DBM->Find(this->key);
 
      if (!userdb)
      {
            user->SendProtocol(ERR_QUERY, PROCESS_NULL);
      }
      else
      {
            if (userdb == user->GetDatabase())
            {
                  user->SetNullDB();
            }
  
            sfalert(user, NOTIFY_DEFAULT, "Removed database: %s", userdb->GetName().c_str());
            FileSystem::remove_directory(userdb->GetPath().c_str());
         
            Kernel->Store->DBM->Delete(this->key);  
            user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
      }
}
