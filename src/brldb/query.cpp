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
#include "brldb/query.h"

void query_base::DelExpire()
{
        /* Deletes key in case it is expiring. */
        
        Kernel->Store->Expires->Delete(this->database, this->key, this->select_query);

        const std::string& exp_where = to_bin(this->key) + ":" + this->select_query + ":" + INT_EXPIRE;
        this->database->GetAddress()->Delete(rocksdb::WriteOptions(), exp_where);
}

void query_base::SetRegistry(const std::string& select, const std::string& base, const std::string& key)
{
       const std::string& save = key + ":" + select + ":" + INT_REG;
       this->database->GetAddress()->Put(rocksdb::WriteOptions(), save, base);
}

std::string query_base::GetRegistry(const std::string& select, const std::string& key)
{
       const std::string& saved = key + ":" + select + ":" + INT_REG;

       std::string dbvalue;
       rocksdb::Status fstatus2 = this->database->GetAddress()->Get(rocksdb::ReadOptions(), saved, &dbvalue);

       if (fstatus2.ok())
       {
              return dbvalue;
       }

       return "";
}

void query_base::RemoveRegistry(const std::string& select, const std::string& base, const std::string& key)
{
       const std::string& save = key + ":" + select + ":" + INT_REG;
       this->database->GetAddress()->Delete(rocksdb::WriteOptions(), save);
}

bool query_base::Check()
{
       if (!this->database)
       {
             this->access_set(DBL_STATUS_BROKEN);
             return false;
       }
       else
       {
             this->access_set(DBL_STATUS_OK);
       }
       
       return true;
}

bool query_base::CheckKey()
{
      if (this->key_required)
      {
             if (this->key.empty())
             {
                  this->access_set(DBL_MISS_ARGS);
                  return false;
             }
             else
             {
                  return true;
             }
      }
      
      return true;
}