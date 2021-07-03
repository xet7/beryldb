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
#include "brldb/dbmanager.h"
#include "managers/lists.h"
#include "managers/settings.h"

DBManager::DBManager()
{

}

std::shared_ptr<UserDatabase> DBManager::Load(const std::string& name, const std::string& path)
{
        if (name.empty() || path.empty())
        {
            return nullptr;
        }

        std::shared_ptr<UserDatabase> New = nullptr;
        New = std::make_shared<UserDatabase>(name, path);

        New->created = Kernel->Now();
        this->DBMap.insert(std::make_pair(name, std::shared_ptr<UserDatabase>(New)));
        bprint(DONE, "Adding database: %s", name.c_str());
        New->Open();
        return New;
}

bool DBManager::Delete(const std::string& name)
{
      /* Name does not exists. */
      
      std::shared_ptr<UserDatabase> userdb = this->Find(name);
      
      if (!userdb)
      {
            return false;
      }
      
      userdb->Close();
//      STHelper::Delete("databases", name);
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
      
      if (!Kernel->Engine->IsDatabase(name))
      {
           return false;
      }
      
//      std::string realpath = path + ".db";
      //ListHelper::Add("databases", "databases", name);
//      STHelper::Set("databases", name, realpath);
      return true;
}

void DBManager::SetDefault(const std::string& name)
{
  //     STHelper::Set("databases", "dbdefault", name);
}

unsigned int DBManager::OpenAll()
{
       //this->Create("default", "default");
       Kernel->Store->Default = this->Load("default", "default");
       return 1;
       
/*       VectorTuple listvector = ListHelper::Get("databases", "databases");
       Args dblist = std::get<1>(listvector);
       
       unsigned int counter = 0;

       std::string dbdefault = STHelper::Get("databases", "dbdefault");

       for (Args::iterator i = dblist.begin(); i != dblist.end(); i++)
       {
             std::string database = *i;
             std::string path = STHelper::Get("databases", database.c_str());

             if (dbdefault == database)
             {
                   Kernel->Store->GetDefault() = this->Load(database, path);
             }
             else
             {
                   this->Load(database, path);
             }
             
             counter++;
       }
       
       return counter;*/
}

std::shared_ptr<UserDatabase> DBManager::Find(const std::string& name)
{
        DataMap::iterator it = this->DBMap.find(name);

        if (it == this->DBMap.end())
        {
                return nullptr;
        }

        return it->second;
}


