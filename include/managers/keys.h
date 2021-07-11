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

#include "brldb/database.h"

class ExportAPI KeyHelper
{
    public:

        static void WDelete(User* user, const std::string& key);

        static void Touch(User* user, const std::string& entry);

        static void NTouch(User* user, const std::string& entry);

        static void Rename(User* user, const std::string& entry, const std::string& value);

        static void Copy(User* user, const std::string& entry, const std::string& value);

        static void GetSet(User* user, const std::string& entry, const std::string& value);

        static void Count(User* user, const std::string& key);

        static void Strlen(User* user, const std::string& key);
        
        static void GetDel(User* user, const std::string& key);
        
        static void Exists(User* user, const std::string& key);

        static void Search(User* user, const std::string& key, signed int offset, signed int limit);

        static void Keys(User* user, const std::string& key, signed int offset, signed int limit);

        static void Count(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& customreply = "");

        static void Get(User* user, const std::string& key);

        static void Operation(User* user, const std::string& key, OP_TYPE type, const std::string& oper = "");
    
        static void Delete(User* user, const std::string& key);

        static void Set(User* user, const std::string& entry, const std::string& value);

        static void SetNX(User* user, const std::string& entry, const std::string& value);

        static void GetSubstr(User* user, const std::string& key, signed int offset, signed int limit);
        
        static void Append(User* user, const std::string& key, const std::string& value);

        static void SetTX(User* user, const std::string& entry, const std::string& value);
        
        static void GetOccurs(User* user, const std::string& key, const std::string& value);

        static void Random(User* user);

        static void GetExp(User* user, const std::string& entry, const std::string& seconds);

        static void Alpha(User* user, const std::string& entry);
        
        static void IsNum(User* user, const std::string& entry);
        
        
};
