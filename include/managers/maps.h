
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
#include "brldb/hquery.h"

class ExportAPI MapsHelper
{
    public:

       static void Exists(User* user, const std::string& entry, const std::string& hesh);

       static void Move(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& hesh, const std::string& dest);

       static BasicTuple Get(const std::string& where, const std::string& key, const std::string& hesh);

       static DBL_CODE Set(const std::string& where, const std::string& entry, const std::string& val, const std::string& hesh);
        
       static DBL_CODE Delete(const std::string& where, const std::string& entry, const std::string& value);

       static VectorTuple Search(QUERY_TYPE type, const std::string& where, const std::string& key, signed int offset, signed int limit);
       
       static void Get(User* user, const std::string& entry, const std::string& hesh);

       static void Set(User* user, const std::string& entry, const std::string& hesh, const std::string& value);

       static void Delete(User* user, const std::string& entry, const std::string& hesh);

       static void Search(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, signed int offset, signed int limit);
       
       static void Count(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key);
       
       static VectorTuple HKeys(const std::string& where, const std::string& key);

       static DBL_CODE DeleteAll(const std::string& where, const std::string& entry);
       
       static void DeleteAll(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry);

       static void SearchHesh(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& hesh, signed int offset, signed int limit);

       static MMapTuple SearchHesh(const std::string& where, const std::string& hesh, signed int offset = 0, signed int limit = -1);
       
       static void HKeys(User* user, const std::string& entry, signed int offset, signed int limit);

};
