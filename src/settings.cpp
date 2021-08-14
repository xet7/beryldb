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
#include "settings.h"
#include "helpers.h"
#include "managers/user.h"
#include "managers/settings.h"
#include "managers/maps.h"
#include "brldb/dbmanager.h"
#include "subscription.h"

Settings::Settings() 
{

}

void Settings::SetDefaults()
{
       this->defaults.clear();
       this->SetMap.clear();
       
       STHelper::Erase("conf");       
       
       defaults.insert({ "allowchans", "true" });
       defaults.insert({ "syntaxhints", "true" });
       defaults.insert({ "autojoin", "false" });
       defaults.insert({ chansync, "false" });

      for (std::map<std::string, std::string> ::iterator i = this->defaults.begin(); i != this->defaults.end(); i++)      
      {
             std::string key = i->first;
             std::string value = i->second;
             this->Set(key, value);
      }
}

void Settings::Load()
{
        const Args& confs = STHelper::HKeys("conf");

        for (Args::const_iterator i = confs.begin(); i != confs.end(); i++)
        {
              std::string key = *i;
              this->SetMap[key] = STHelper::Get("conf", key);
        }
        
        bprint(DONE, "Settings loaded.");
}

void Settings::Set(std::string& key, std::string& value)
{
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);      
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);      
      
        this->SetMap[key] = value;
        STHelper::Set("conf", key, value);
}

std::string Settings::Get(std::string& key)
{
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);      
        std::map<std::string, std::string>::iterator it = this->SetMap.find(key);

        if (it == this->SetMap.end())
        {
                return "";
        }
        
        return it->second;
}

bool Settings::AsBool(std::string& key)
{
        return Helpers::as_bool(this->Get(key), false);
}

std::string Helpers::to_bool(bool boolean)
{
        if (boolean)
        {
                return "true";
        }
        else
        {
                return "false";
        }
}

bool Helpers::as_bool(const std::string &key, bool def)
{
        const std::string& result = key;

        if (key.empty())
        {
                return def;
        }

        if (stdhelpers::string::equalsci(result, "yes") || stdhelpers::string::equalsci(result, "true") || stdhelpers::string::equalsci(result, "on") || result == "1")
        {
                return true;
        }

        if (stdhelpers::string::equalsci(result, "no") || stdhelpers::string::equalsci(result, "false") || stdhelpers::string::equalsci(result, "off") || result == "0")
        {
                return false;
        }

        return def;
}

std::string Helpers::Format(const std::string& fmt)
{
    return Daemon::Format("\"%s\"", fmt.c_str());
}

void Helpers::make_query(User* user, std::shared_ptr<QueryBase> base, const std::string& key, bool allow)     
{
      base->user = user;
      base->database = user->GetDatabase();
      base->select_query = user->select;
      
      if (!key.empty())
      {
            base->key = key;
            
            if (!allow)
            {
                  if (!Kernel->Engine->ValidKey(key))
                  {
                      base->access_set(DBL_INVALID_FORMAT);
                  }
            }
      }
}

void Helpers::make_geo_query(User* user, std::shared_ptr<QueryBase> base, const std::string& key)
{
      base->user = user;
      base->database = user->GetDatabase();
      base->select_query = user->select;

      if (!key.empty())
      {
            base->key = key;
      }
}

void Helpers::make_map(User* user, std::shared_ptr<QueryBase> base, const std::string& key, const std::string& hesh, bool allow)
{
     base->user = user;
     base->database = user->GetDatabase();
     base->select_query = user->select;
     
     if (!key.empty() && !hesh.empty())
     {
            base->key = key;
            base->hesh = hesh;

            if (!allow)
            {
                  if (!Kernel->Engine->ValidKey(key))
                  {
                      base->access_set(DBL_INVALID_FORMAT);
                  }
            }

     }
}

void Helpers::make_cmap(std::shared_ptr<QueryBase> base, const std::string& key, const std::string& hesh)
{
       base->database = Kernel->Core->GetDatabase();
       base->select_query = "1";
       base->flags = QUERY_FLAGS_CORE;
     
       if (!key.empty() && !hesh.empty())
       {
            base->key = key;
            base->hesh = hesh;

            if (!Kernel->Engine->ValidKey(key))
            {
                  base->access_set(DBL_INVALID_FORMAT);
            }
       }
}

void Helpers::make_cquery(std::shared_ptr<QueryBase> base, const std::string& key)
{
      base->database = Kernel->Core->GetDatabase();
      base->select_query = "1";
      base->flags = QUERY_FLAGS_CORE;
      
      if (!key.empty())
      {
            base->key = key;
            
            if (!Kernel->Engine->ValidKey(key))
            {
                  base->access_set(DBL_INVALID_FORMAT);
            }
      }
}

void Helpers::make_mmap(User* user, std::shared_ptr<QueryBase> base, const std::string& key, bool allow)
{
     base->user = user;
     base->database = user->GetDatabase();
     base->select_query = user->select;
     
     if (!key.empty())
     {
            base->key = key;

            if (!allow)
            {
                  if (!Kernel->Engine->ValidKey(key))
                  {
                      base->access_set(DBL_INVALID_FORMAT);
                  }
            }
     }
}

void Helpers::make_list(User* user, std::shared_ptr<QueryBase> base, const std::string& key, bool allow)     
{
      base->user = user;
      base->database = user->GetDatabase();
      base->select_query = user->select;
      
      if (!key.empty())
      {
            base->key = key;

            if (!allow)
            {
                  if (!Kernel->Engine->ValidKey(key))
                  {
                      base->access_set(DBL_INVALID_FORMAT);
                  }
            }
      }
}

std::string Helpers::TypeString(const std::string& type, bool plural)
{
     if (type.empty() || type == PROCESS_NULL)
     {
          return PROCESS_NULL;
     }
     
     if (type == INT_KEY)
     {
           return Daemon::Format("KEY%s", plural == true ? "S" : "");
     }
     
     if (type == INT_GEO)
     {
           return Daemon::Format("GEO%s", plural == true ? "S" : "");
     }
     
     if (type == INT_LIST)
     {
           return Daemon::Format("LIST%s", plural == true ? "S" : "");
     }
     
     if (type == INT_MAP)
     {
           return Daemon::Format("MAP%s", plural == true ? "S" : "");
     }
     
     if (type == INT_VECTOR)
     {
           return Daemon::Format("VECTOR%s", plural == true ? "S" : "");
     }
     
     if (type == INT_MMAP)
     {
           return Daemon::Format("MULTIMAP%s", plural == true ? "S" : "");
     }
     
     if (type == INT_FUTURE)
     {
           return Daemon::Format("FUTURE%s", plural == true ? "S" : "");
     }
     
     if (type == INT_EXPIRE)
     {
           return Daemon::Format("EXPIRE%s", plural == true ? "S" : "");
     }
     
     return "UNKNOWN";
}


