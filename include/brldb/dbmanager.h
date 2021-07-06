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
      
      void CloseAll();
      
      void Load(const std::string& name, bool defaultdb = false);

      std::shared_ptr<UserDatabase> Find(const std::string& name);

      static void SetDefault(const std::string& name);

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

        void Push(std::shared_ptr<QueryBase> request);
        
        void SetDefault(const std::shared_ptr<UserDatabase>& db)
        {
              this->Default = db;
        }
        
        std::shared_ptr<UserDatabase> GetDefault()
        {
              return this->Default;
        }

};