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
#include "subscription.h"

Settings::Settings() 
{

}

void Settings::SetDefaults()
{
       defaults.insert({ "allowchans", "true" });
       defaults.insert({ "syntaxhints", "true" });
       defaults.insert({ "autojoin", "false" });
       defaults.insert({ "chansync", "false" });

      for (std::map<std::string, std::string> ::iterator i = this->defaults.begin(); i != this->defaults.end(); i++)      
      {
             std::string key = i->first;
             std::string value = i->second;
             this->Set(key, value);
      }
}

void Settings::Load()
{
//        bprint(INFO, "Loading settings.");
  
 /*       VectorTuple tpl = //MapsHelper::HKeys(TABLE_SETTINGS, "conf");
        Args confs = std::get<1>(tpl);
        
        for (Args::iterator i = confs.begin(); i != confs.end(); i++)
        {
             std::string key = *i;
             this->SetMap[key] = STHelper::Get("conf", key);
        }*/
}

void Settings::Set(const std::string& key, const std::string& value)
{
      this->SetMap[key] = value;
//      STHelper::Set("conf", key, value);
}

std::string Settings::Get(const std::string& key)
{
        std::map<std::string, std::string>::iterator it = this->SetMap.find(key);

        if (it == this->SetMap.end())
        {
                return "";
        }
        
        return it->second;
}

bool Settings::AsBool(const std::string& key)
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
        std::string result = key;

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

void Helpers::make_query(User* user, std::shared_ptr<query_base> base, const std::string& key)     
{
      base->user = user;
      base->database = user->current_db;
      base->select_query = user->select;
      
      if (!key.empty())
      {
            base->key = key;
            base->format = key + base->select_query; 
            base->dest = to_bin(key) + ":" + base->select_query + ":" + base->base_request;
      }
}

void Helpers::make_map(User* user, std::shared_ptr<query_base> base, const std::string& key, const std::string& hesh)
{
     base->user = user;
     base->database = user->current_db;
     base->select_query = user->select;
      
     if (!key.empty() && !hesh.empty())
     {
            base->key = key;
            base->hesh = hesh;
            base->format = key + base->select_query; 
            base->dest = to_bin(base->key) + ":" + base->select_query + ":" + base->base_request + ":" + to_bin(base->hesh);
     }
}


std::string Helpers::TypeString(const std::string& type)
{
     if (type == "1")
     {
          return "KEY";
     }
     
     if (type == "2")
     {
         return "MAP";
     }
}


