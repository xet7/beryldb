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
#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "managers/maps.h"
#include "managers/user.h"

bool UserHelper::CheckPass(const std::string& user, const std::string& key)
{
   	/* Wrong password. */
   	
        signed int cached = Kernel->Logins->InCache(user, key);
        
        if (cached == -1)
        {
                return false;
        }
        else if (cached == 1)
        {
                return true;
        }

        BasicTuple tuple = MapsHelper::Get(TABLE_USERS, user, "pass");
        
        const std::string provided_pass = std::get<1>(tuple);

        if (provided_pass != key)
        {
             return false;
        }
        
        /* We may add this login to the cache. */
        
        Kernel->Logins->AddCache(user, provided_pass);
        return true;
}

std::string UserHelper::Find(const std::string& user, const std::string& key)
{
        BasicTuple tuple = MapsHelper::Get(TABLE_USERS, user, key);
        return std::get<1>(tuple);
}

bool UserHelper::Add(const std::string& user, const std::string& pass)
{
        /* Min length is 3 for an user. */
        
        if (user.length() < 3 || pass.length() < 3)
        {
            return false;
        }
        
        MapsHelper::Set(TABLE_USERS, user, "userlogin", user);
        MapsHelper::Set(TABLE_USERS, user, "pass", pass);
        return MapsHelper::Set(TABLE_USERS, user, "created", convto_string(Kernel->Now()));
}

bool UserHelper::ChangePass(const std::string& user, const std::string& pass)
{
       if (pass.length() < 3 || pass.length() > 30)
       {
            return false;
       }
      
       /* 
        * Removes the user from the cache so the 'current' password will
        *  not continue working.
        */
        
       Kernel->Logins->RemoveCache(user); 
       
       return MapsHelper::Set(TABLE_USERS, user, "pass", pass);
}

bool UserHelper::AddAdmin(const std::string& user, const std::string& flags)
{
        MapsHelper::Set(TABLE_ADMIN, user, "adminlogin", user);
        return MapsHelper::Set(TABLE_ADMIN, user, "flags", flags);
}

bool UserHelper::SetAdmin(const std::string& user, const std::string& flags)
{
      if (flags.empty() || flags == "")
      {
            MapsHelper::DeleteAll(TABLE_ADMIN, user);
      }
      
      return MapsHelper::Set(TABLE_ADMIN, user, "flags", flags);
}

bool UserHelper::AdminExists(const std::string& user)
{
        BasicTuple tuple = MapsHelper::Get(TABLE_ADMIN, user, "flags");
        const std::string flags = std::get<1>(tuple);
        
        if (flags.empty() || flags == "")
        {
            return false;
        }
        
        return true;
}

std::string UserHelper::FindAdmin(const std::string& user)
{
        BasicTuple tuple = MapsHelper::Get(TABLE_ADMIN, user, "flags");
        const std::string flags = std::get<1>(tuple);
        return flags;
}

bool UserHelper::RemoveAdmin(const std::string& user)
{
        return MapsHelper::DeleteAll(TABLE_ADMIN, user);
}

bool UserHelper::Remove(const std::string& user)
{
        Kernel->Logins->RemoveCache(user); 

        MapsHelper::DeleteAll(TABLE_ADMIN, user);
        return MapsHelper::DeleteAll(TABLE_USERS, user);
}

bool UserHelper::Exists(const std::string& user)
{
    std::string created = UserHelper::Find(user, "created");
    
    if (created.empty() || created == "")
    {
             return false;
    }
    
    return true;
}


