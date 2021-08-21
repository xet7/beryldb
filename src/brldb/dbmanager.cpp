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
#include "engine.h"
#include "managers/settings.h"

DBManager::DBManager() 
{

}

void DBManager::CloseAll()
{
        for (DataMap::iterator i = DBMap.begin(); i != DBMap.end(); ++i)
        {
                 std::shared_ptr<UserDatabase> db = i->second;
                 db->SetClosing(true);
                 db->Close();
        }
}

void DBManager::Load(const std::string& name, bool dbdefault)
{
        if (name.empty())
        {
             return;
        }

        std::string path = STHelper::Get("databases", name);

        std::shared_ptr<UserDatabase> New  = NULL;
        New 				   = std::make_shared<UserDatabase>(name, path);
        New->created 			   = Kernel->Now();
        
        this->DBMap.insert(std::make_pair(name, std::shared_ptr<UserDatabase>(New)));
        New->Open();
        bprint(DONE, "Initializing database: %s", name.c_str());
        
        if (dbdefault)
        {
              Kernel->Store->SetDefault(New);
        }
}

bool DBManager::Delete(const std::string& name)
{
      /* Name does not exists. */
      
      const std::shared_ptr<UserDatabase>& userdb = this->Find(name);
      
      if (!userdb)
      {
            return false;
      }
      
      userdb->Close();
      STHelper::Delete("databases", name);
      this->DBMap.erase(name);
      return true;
}

bool DBManager::Create(const std::string& name, const std::string& path)
{
      /* Name exists. */
      
      if (this->Find(name))
      {
            return false;
      }
      
      if (this->FindPath(path))
      {
            return false;
      }
      
      if (!Kernel->Engine->IsDatabase(name))
      {
           return false;
      }
            
      std::string realpath = path + ".db";
      STHelper::Set("databases", name, realpath);
      return true;
}

void DBManager::SetDefault(const std::string& name)
{
       STHelper::Set("dbconf", "dbdefault", name);
}

unsigned int DBManager::OpenAll()
{
       unsigned int counter = 0;
       
       const Args& dblist = STHelper::HList("databases");
       const std::string& dbdefault = STHelper::Get("dbconf", "dbdefault");

       for (Args::const_iterator i = dblist.begin(); i != dblist.end(); i++)
       {
             std::string name = *i;

             if (dbdefault == name)
             {
                   bprint(DONE, "Setting default database: %s", name.c_str());
                   this->Load(name, true);
             }
             else
             {
                   this->Load(name);
             }
             
             counter++;
       }

       return counter;
}

std::shared_ptr<UserDatabase> DBManager::FindPath(const std::string& path)
{
        std::string dbpath = Kernel->Config->Paths->SetWDDB(path) + ".db";
        std::transform(dbpath.begin(), dbpath.end(), dbpath.begin(), ::tolower);

        for (DataMap::iterator i = DBMap.begin(); i != DBMap.end(); ++i)
        {
                 std::shared_ptr<UserDatabase> db = i->second;
                 
                 if (i->second->GetPath() == dbpath)
                 {
                       return db;
                 }
        }
        
        return NULL;
}

std::shared_ptr<UserDatabase> DBManager::Find(const std::string& name)
{
        std::string dbname = name;
        std::transform(dbname.begin(), dbname.end(), dbname.begin(), ::tolower);

        DataMap::iterator it = this->DBMap.find(dbname);

        if (it == this->DBMap.end())
        {
                return NULL;
        }

        return it->second;
}


