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

void del_query::Keys()
{
       const std::string& lookup = to_bin(this->key) + ":" + this->select_query + ":" + INT_EXPIRE + ":" + this->database->GetName();

       rocksdb::WriteBatch batch;

       batch.Delete(this->dest);
       batch.Delete(lookup);
       
       rocksdb::Status stats = this->database->GetAddress()->Write(rocksdb::WriteOptions(), &batch);

       if (stats.ok())
       {
             Kernel->Store->Expires->Delete(this->database, this->key, this->select_query);
             this->SetOK();
             return;
       }
       
       access_set(DBL_BATCH_FAILED);
}

void del_query::Lists()
{

}

void del_query::Multis()
{

}

void del_query::Maps()
{

}

void del_query::Geos()
{

}

void del_query::Vectors()
{

}

void del_query::Run()
{
    if (this->identified == INT_KEY)
    {
          signed int ttl = this->IsExpiring();

          if (this->IsExpiring() > 0)
          {
              this->id = ttl;
              this->Keys();
              return;    
          }
    } 
    else if (this->identified == INT_MAP)
    {
          this->Maps();
    }
    else if (this->identified == INT_GEO)
    {
          this->Geos();
    }
    else if (this->identified == INT_MMAP)
    {
          this->Multis();
    }
    else if (this->identified == INT_LIST)
    {
         this->Lists();
    }
    else if (this->identified == INT_VECTOR)
    {    
          this->Vectors();
    }   

    this->Delete(this->dest);
    this->SetOK();
}

void del_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}
