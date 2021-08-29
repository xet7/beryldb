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
#include "group.h"

class ExportAPI DBManager : public safecast<DBManager>
{
   private:
     
        DataMap DBMap;

   public:
       
        /* Constructor */
         
        DBManager();

        /* 
         * Opens all databases.
	 * 
         * @return:
 	 *
         *         · uint: Counter of opened databases.
         */             
         
        unsigned int OpenAll();
      
        void CloseAll();
      
        void Load(const std::string& name, bool defaultdb = false);

        /* 
         * Finds a database within DBMap.
         * 
         * @parameters:
	 *
	 *         · name: Database to lookup.
	 * 
         * @return:
 	 *
         *         · Database ptr to UserDatabase. Returns NULL if not found.
         */    
         
        std::shared_ptr<UserDatabase> Find(const std::string& name);

        static void SetDefault(const std::string& name);

        /* 
         * Deletes a database from DBMap.
         * 
         * @parameters:
	 *
	 *         · string: Database to remove.
	 * 
         * @return:
 	 *
         *         · True: Database found and removed.
         */    
         
        bool Delete(const std::string& name);
        
        /* 
         * Creates a new database.
         * 
         * @parameters:
	 *
	 *         · string	: Name of database.
	 * 
         * @return:
 	 *
         *         · True	: Database created.
         */             
         
        bool Create(const std::string& name, const std::string& path);

        /* 
         * Returns a database based on its path.
         * 
         * @parameters:
         *
         *         · string       :     Path to find.
         * 
         * @return:
         *
         *         · UserDatabase :    Database, if found. Returns NULL otherwise.
         */

        std::shared_ptr<UserDatabase> FindPath(const std::string& path);
        
        /* 
         * Get database.
         * 
         * @return:
 	 *
         *         · DataMap. 
         */             
         
        DataMap& GetDatabases()
        {
               return this->DBMap;
        }
        
        /* 
         * Counts databases opened.
	 * 
         * @return:
 	 *
         *         · uint	:	Databases running.
         */            
         
        unsigned int CountDatabases()
        {
              return this->DBMap.size();
        }
};

class ExportAPI CoreManager 
{
    private:
    
         /* Core database pointer. This is unique. */
         
         std::shared_ptr<CoreDatabase> DB;

    public:
    
        /* Constructor */
        
        CoreManager();

        /* Opens core database. */
        
        void Open();
        
        void SetDefaults();
        
        /* Checks if defaults have been set. */
        
        void CheckDefaults();
        
        void UserDefaults();

        /* 
         * Returns a reference to the core database.
         * 
         * @return:
 	 *
         *         · this->DB
         */            
         
        std::shared_ptr<CoreDatabase>& GetDatabase()
        {
             return this->DB;
        }
};

class ExportAPI StoreManager 
{
    friend class DBManager;
    friend class CoreManager;
    
    private:

        rocksdb::Env* env = NULL;
    
        /* Default database to use */
        
        std::shared_ptr<UserDatabase> Default;

        /* Time this instance was created. */

        time_t created;

        /* Whether this is the first time this instance is ran. */

        bool First;

    public:

        /* Constructor. */

        StoreManager();
        
        /* Destructor */
        
        ~StoreManager();

        /* Map of active databases. */
        
        DBManager DBM;

        /* Handles expiring keys. */
        
        ExpireManager Expires;
        
        /* Handles futures. */
        
        FutureManager Futures;
        
        /* Flusher class */
        
        DataFlush Flusher; 
        
        /* Opens threads. */

        void OpenAll();

        void Push(std::shared_ptr<QueryBase> request);

        /* 
         * Returns whether this is the first time this instance was ran.
	 * 
         * @return:
 	 *
         *         · True: First time ran.
         */            
         
        bool IsFirst()
        {
              return this->First;
        }

        /* 
         * Returns created time.
         * 
         * @return:
 	 *
         *         · time_t: Created time.
         */            
         
        time_t GetCreated()
        {
              return this->created;
        }
        
        void SetDefault(const std::shared_ptr<UserDatabase>& db)
        {
              this->Default = db;
        }
        
        /* 
         * Returns default database used.
	 * 
         * @return:
 	 *
         *         · UserDatabase	: Default database.
         */            
         
        std::shared_ptr<UserDatabase> GetDefault()
        {
              return this->Default;
        }

        /* 
         * Returns current environment variable.
         * 
         * @return:
 	 *
         *         · rocksdb::Env*: env var.
         */            
         
        rocksdb::Env* GetEnv()
        {
              return this->env;
        }

};