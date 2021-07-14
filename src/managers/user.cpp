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
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "settings.h"
#include "managers/maps.h"
#include "managers/user.h"
#include "modules/encrypt.h"

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

        const std::string passwd = CMapsHelper::Get(user, "pass").response;
        
        HashProvider* provider = Kernel->Modules->DataModule<HashProvider>("hash/bcrypt");

        if (!provider)
        {
               return false;
        }

        if (!provider->Compare(key, passwd))
        {
             return false;
        }

        /* We may add this login to the cache. */

        Kernel->Logins->Add(user, passwd);
        return true;
}

std::string UserHelper::Find(const std::string& key, const std::string& value)
{
        return CMapsHelper::Get(key, value).response;
}

bool UserHelper::Add(const std::string& user, const std::string& pass)
{
        /* Min length is 3 for an user. */
        
        if (user.length() < 3 || user.length() > 15)
        {
            return false;
        }

        HashProvider* provider = Kernel->Modules->DataModule<HashProvider>("hash/bcrypt");
        
        if (!provider)
        {
               return false;
        }

        std::string hashed_pass = provider->Generate(pass);
            
        CMapsHelper::Set(user, "userlogin", user);
        CMapsHelper::Set(user, "pass", hashed_pass);
        CMapsHelper::Set(user, "created", convto_string(Kernel->Now()));

        CMapsHelper::Set("userlist", user, "login");
        
        const std::string& usergrups = user + "/groups";
        
        CMapsHelper::Set(usergrups, "default", convto_string(Kernel->Now()));
        
//        NOTIFY_MODS(OnUserAdd, (key, value));
        
        return true;
}

bool UserHelper::ChangePass(const std::string& user, const std::string& pass)
{
       if (pass.length() < 3 || pass.length() > 30)
       {
            return false;
       }
      
       /* 
        * Removes the user from the cache so the 'current' password will
        * not be accepted if an user tries to log-in utilizing same
        * password.
        */

        Kernel->Logins->Remove(user); 

        HashProvider* provider = Kernel->Modules->DataModule<HashProvider>("hash/bcrypt");

        if (!provider)
        {
               return false;
        }

        std::string hashed_pass = provider->Generate(pass);
        CMapsHelper::Set(user, "pass", hashed_pass);
        return true;
}

bool UserHelper::SetFlags(const std::string& user, const std::string& flags)
{
        CMapsHelper::Set(user, "flags", flags);
        return true;
}

bool UserHelper::HasFlags(const std::string& user)
{
        const std::string flags = CMapsHelper::Get(user, "flags").response;
        
        if (flags.empty() || flags == "")
        {
            return false;
        }

        return true;
}

std::string UserHelper::CheckFlags(const std::string& user)
{
        return CMapsHelper::Get(user, "flags").response;
}

bool UserHelper::DeleteFlags(const std::string& user)
{
        CMapsHelper::Del(user, "flags");
        return true;
}

bool UserHelper::Remove(const std::string& user)
{
        Kernel->Logins->Remove(user); 
        
        CMapsHelper::Del(user, "pass");
        CMapsHelper::Del(user, "created");
        CMapsHelper::Del(user, "flags");
        CMapsHelper::Del(user, "userlogin");      
        CMapsHelper::Del("userlist", user);

        std::string usergrups = user + "/groups";
        CMapsHelper::Erase(usergrups);
        return true;
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


