/*
 * BerylDB - A modular database.
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

class Externalize UserHelper
{
    public:
    
        static std::string Find(const std::string& user, const std::string& key);

        static bool Add(const std::string& user, const std::string& pass);

        static bool Remove(const std::string& user);

        static bool Exists(const std::string& user);

        static bool AddAdmin(const std::string& user, const std::string& flags);

        static bool RemoveAdmin(const std::string& user);

        static bool CheckPass(const std::string& user, const std::string& key);

        static std::string FindAdmin(const std::string& user);

        static bool AdminExists(const std::string& user);

        static bool SetAdmin(const std::string& user, const std::string& flags);
        

        /* Changes an user password. */
        
        static bool ChangePass(const std::string& user, const std::string& pass);

};