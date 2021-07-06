
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
#include "cstruct.h"

class ExportAPI MapsHelper
{
    public:

       static void WDelete(User* user, const std::string& key, const std::string& hesh);

       static void Move(User* user, const std::string& entry, const std::string& hesh, const std::string& value);

       static void Exists(User* user, const std::string& entry, const std::string& hesh);

       static void Get(User* user, const std::string& entry, const std::string& hesh);

       static void Set(User* user, const std::string& entry, const std::string& hesh, const std::string& value);

       static void Delete(User* user, const std::string& entry, const std::string& hesh);

       static void Search(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, signed int offset, signed int limit);
       
       static void Count(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key);
       
       static void Erase(User* user, const std::string& entry);

       static void SearchHesh(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& hesh, signed int offset, signed int limit);

       static void List(User* user, const std::string& entry, signed int offset, signed int limit);
       
       static void Count(User* user, const std::string& entry);
       
       static void Strlen(User* user, const std::string& entry, const std::string& hesh);
       
       static void SetNX(User* user, const std::string& entry, const std::string& hesh, const std::string& value);

       static void Find(User* user, const std::string& entry, signed int offset, signed int limit);

};

class ExportAPI CMapsHelper
{
    public:
       
       static MapData Set(const std::string& entry, const std::string& hesh, const std::string& value);
       
       static MapData Get(const std::string& key, const std::string& hesh);

       static MapData Del(const std::string& key, const std::string& hesh);

       static MapData HKeys(const std::string& entry);
       
       static MapData Erase(const std::string& entry);
};