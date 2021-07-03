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

#pragma once

#include "brldb/dbflush.h"
#include "brldb/expires.h"
#include "brldb/futures.h"
#include "brldb/database.h"

class ExportAPI DBManager : public safecast<DBManager>
{
   private:
     
      DataMap DBMap;

   public:
       
      /* Constructor */
         
      DBManager();
         
      unsigned int OpenAll();

      std::shared_ptr<UserDatabase> Load(const std::string& name, const std::string& path);

      std::shared_ptr<UserDatabase> Find(const std::string& name);

      void SetDefault(const std::string& name);

      bool Delete(const std::string& name);
         
      bool Create(const std::string& name, const std::string& path);
         
      DataMap& GetDatabases()
      {
               return this->DBMap;
      }
};

class ExportAPI CoreManager : public safecast<CoreManager>
{
    public:
    
        CoreManager();

        std::shared_ptr<CoreDatabase> DB;
        
        /* Opens core database. */
        
        void Open();
        
        void SetDefaults();
        
        /* Checks if defaults have been set. */
        
        void CheckDefaults();
        
        void UserDefaults();
};

class ExportAPI StoreManager : public safecast<StoreManager>
{
    friend class DBManager;
    
    private:
    
         std::shared_ptr<UserDatabase> Default;

    public:

        rocksdb::Env* env = NULL;
            
        /* Whether this is the first time this instance is ran. */
        
        bool First;
        
        /* Time this instance was created. */
        
        time_t instance;
        
        /* Map of active databases. */
        
        DBManager DBM;

        /* Handles expiring keys. */
        
        ExpireManager Expires;
        
        /* Handles futures. */
        
        FutureManager Futures;
        
        /* Constructor. */
        
        StoreManager();

        /* Flusher class */
        
        DataFlush Flusher; 
        
        /* Opens threads. */

        void OpenAll();

        void Push(std::shared_ptr<query_base> request);
        
        std::shared_ptr<UserDatabase> GetDefault()
        {
              return this->Default;
        }

};