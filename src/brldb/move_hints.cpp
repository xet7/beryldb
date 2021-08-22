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
#include "helpers.h"

void move_query::Keys()
{
     RocksData result = this->Get(this->dest);
     
     const std::string& newdest = to_bin(this->key) + ":" + this->value + ":" + this->identified;
     const std::string& lookup = to_bin(this->key) + ":" + this->value + ":" + INT_EXPIRE + ":" + this->database->GetName();

     rocksdb::WriteBatch batch;

     batch.Put(newdest, result.value);
     batch.Delete(this->dest);
     batch.Put(lookup, convto_string(this->id));

     rocksdb::Status stats = this->database->GetAddress()->Write(rocksdb::WriteOptions(), &batch);

     if (stats.ok())
     {
             Kernel->Store->Expires->Delete(this->database, this->key, this->select_query);
             Kernel->Store->Expires->Add(this->database, this->id, this->key, convto_num<unsigned int>(this->value), true);
             this->SetOK();
     }
     else
     {
          access_set(DBL_BATCH_FAILED);
     }
}

void move_query::Maps()
{

}

void move_query::Multis()
{

}

void move_query::Geos()
{

}

void move_query::Lists()
{

}

void move_query::Vectors()
{

}

void move_query::Run()
{
    if (this->identified == INT_KEY)
    {
          signed int ttl = this->IsExpiring();

          if (ttl > 0)
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
    else if (this->identified == INT_MMAP)
    {
          this->Multis();
    }
    else if (this->identified == INT_GEO)
    {
          this->Geos();
    }
    else if (this->identified == INT_LIST)
    {
          this->Lists();
    }
    else if (this->identified == INT_VECTOR)
    {    
          this->Vectors();
    }   
    
    RocksData result = this->Get(this->dest);
    const std::string& newdest = to_bin(this->key) + ":" + this->value + ":" + this->identified;
    
    if (!this->Swap(newdest, this->dest, result.value))
    {
          access_set(DBL_BATCH_FAILED);
    }
    else
    {
        this->SetOK();
    }
        
}

void move_query::Process()
{
       user->SendProtocol(BRLD_OK, PROCESS_OK);
}

