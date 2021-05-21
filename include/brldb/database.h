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

#include <rocksdb/write_batch.h>
#include <rocksdb/db.h>
#include <rocksdb/c.h>
#include <rocksdb/options.h>
#include <rocksdb/env.h>

class Database
{
    friend class CoreDatabase;
    friend class UserDatabase;
    friend class CoreManager;
    friend class StoreManager;
    
    private:
    
        /* Pointer to the rocksdb instance. */
        
        rocksdb::DB* db = NULL;
        
        /* Options this DB is working with */
        
        rocksdb::Options options;
        
        /* Opening/Closure status. */
        
        rocksdb::Status status;

        /* Time at which database was created */
        
        time_t created;

        /* Name of this database. */

        std::string name;
        
        /* Path to a database. */
       
        std::string path;
     
    public:

        /* Constructor. */
        
        Database(time_t dbcreated, const std::string& dbname, const std::string& dbpath);

        /* 
         * GetAddress(): Processing database to rocksdb.
	 * 
         * @return:
 	 *
         *         · Returns database address to this->db.
         */    
         
        rocksdb::DB* GetAddress()
        {
             return this->db;
        }
        
        /* Returns current path */
        
        std::string GetPath()
        {
             return this->path;
        }
         
        /* Returns database name. */
        
        std::string GetName()
        {
             return this->name;
        }
        
        /* 
         * Opens a database.
         * 
         * @return:
 	 *
         *         · True: Database opened correctly.
         */            
         
        bool Open();

        /* Remove all content */
        
        bool FlushDB();

        /* Closes database. */
        
        void Close();
};


class UserDatabase : public Database
{

  public:

       UserDatabase(const std::string& dbname, const std::string& dbpath);
};


class CoreDatabase : public Database
{

  public:

     CoreDatabase();
};
