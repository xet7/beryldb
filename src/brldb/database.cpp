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

#include "beryl.h"
#include "exit.h"

#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/datathread.h"
#include "managers/lists.h"
#include "managers/user.h"
#include "managers/databases.h"
#include "managers/settings.h"
#include "managers/maps.h"

Database::Database(time_t dbcreated, const std::string& dbname, const std::string& dbpath) : created(dbcreated), name(dbname), path(Kernel->Config->Paths.PrependDB(dbpath))
{

}

CoreDatabase::CoreDatabase() : Database(Kernel->Now(), "core", "core.db")
{

}

UserDatabase::UserDatabase(const std::string& dbname, const std::string& dbpath) : Database(Kernel->Now(), dbname, dbpath)
{

}

void Database::Close()
{
        slog("DATABASE", LOG_DEFAULT, "Closing database: %s.", this->path.c_str());

        if (!this->db)
        {
            return;
        }

        delete this->db;
}

bool Database::Open()
{	
        this->db = NULL;
  
        options.create_if_missing = true;
        options.IncreaseParallelism(2);
        options.keep_log_file_num = 1;
        options.write_thread_max_yield_usec = 1;
        options.OptimizeLevelStyleCompaction();
        options.enable_thread_tracking = true;
        options.enable_pipelined_write = true;
        options.max_background_flushes = 4;

        this->status = rocksdb::DB::Open(options, this->path, &this->db);
        
        if (!this->status.ok()) 
        {
                bprint(ERROR, "Error while opening database: %s, %s", this->path.c_str(), this->status.ToString().c_str());
                slog("DATABASE", LOG_DEFAULT, "Error while opening database: %s, %s", this->path.c_str(), this->status.ToString().c_str()); 
                Kernel->Exit(EXIT_CODE_DATABASE);
        }
        else
        {
                slog("DATABASE", LOG_DEFAULT, "Database opened: %s: %s", this->path.c_str(), this->status.ToString().c_str());
        }

        return true;
}

bool Database::FlushDB()
{
    if (!this->db)
    {
            return false;
    }

    bprint(DONE, "flushdb requested for: %s.", this->name.c_str());
    slog("DATABASE", LOG_DEFAULT, "flushdb requested for: %s.", this->name.c_str());
    
    /* We must close our database before flushing it. */
    
    this->Close();
    
    rocksdb::Options d_options;
    rocksdb::DestroyDB(this->path, d_options);
    
    ExpireManager::Reset();
    
    /* We report the opening bool. */
    
    return this->Open();
}

CoreManager::CoreManager()
{

}

void CoreManager::Open()
{
       std::shared_ptr<CoreDatabase> New;
       New = std::make_shared<CoreDatabase>();
       New->created = Kernel->Now();
       //bprint(DONE, "Core database opened.");
       New->Open();
       this->DB = New;
}

void CoreManager::UserDefaults()
{
     if (!Kernel->Store->First)
     {
         return;
     }
     
     /* We add our default user. */
  
     UserHelper::Add("root", "default");
     UserHelper::AddAdmin("root", "r");
}

void CoreManager::CheckDefaults()
{
       std::string result = STHelper::Get("instance", "first_ran");
       
       /* Creating instance, unless otherwise specified. */
       
       time_t instance = Kernel->Now();

       /* No entry set. */
       
       if (result.empty() || result == "")
       {
                 STHelper::Set("instance", "first_ran", convto_string(instance));
                      
                 bprint(DONE, "Welcome to Beryl.");
                 bprint(INFO, "Creating defaults.");

                 /* Default settings. */
                 
                 STHelper::Set("conf", "allowchans", "true");
                 STHelper::Set("conf", "syntaxhints", "true");
                 STHelper::Set("conf", "autojoin", "off");
                 STHelper::Set("conf", "chansync", "off");
                 
                 Kernel->Store->First = true;
       }
       else
       {		
               instance = convto_num<time_t>(result);
               bprint(DONE, "Instance created: %s", Daemon::HumanEpochTime(instance).c_str());
               Kernel->Store->First = false;
       }
       
       Kernel->Store->instance = instance;
}

StoreManager::StoreManager() : First(false)
{

}

void StoreManager::OpenAll()
{
       unsigned int counter = 0;

       for (unsigned int i = 1; i <= 2; i++)
       {
             DataThread* New = new DataThread();   
             New->Create();
             Kernel->Store->Flusher->threadslist.push_back(New);
             counter++;
       }
       
       this->Default = this->Create("default", "default.db");
}

std::shared_ptr<Database> StoreManager::Create(const std::string& name, const std::string& path)
{
        if (name.empty() || path.empty())
        {
            return nullptr;
        }

        std::shared_ptr<UserDatabase> New = nullptr;
        New = std::make_shared<UserDatabase>(name, path);
        
        New->created = Kernel->Now();
        this->databases.insert(std::make_pair(name, std::shared_ptr<UserDatabase>(New)));
        bprint(DONE, "Adding database: %s", name.c_str());
        New->Open();
        this->Default = New;
        return New;
}

void StoreManager::Push(std::shared_ptr<query_base> request)
{
      User* user = request->user;

      if (user == NULL || user->IsQuitting())
      {
           return;
      }

      user->pending.push_back(request);       
}

