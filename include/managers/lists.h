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
#pragma once

#include "brldb/database.h"

class Externalize ListHelper
{
    public:

    static VectorTuple Get(const std::string& where, const std::string& key);

    static DBL_CODE Add(const std::string& where, const std::string& entry, const std::string& val);
        
    static DBL_CODE Delete(const std::string& where, const std::string& entry, const std::string& value, bool onlyfirst = false);
        
    static void Get(User* user, std::shared_ptr<Database> db, const std::string& where, const std::string& key, signed int offset, signed int limit, QUERY_TYPE type = TYPE_NONE);

    static void Add(User* user, std::shared_ptr<Database> db, const std::string& where, const std::string& entry, const std::string& val);
        
    static void Delete(User* user, std::shared_ptr<Database> db, const std::string& where, const std::string& entry, const std::string& value, bool onlyfirst = false);

    static void Exist(User* user, std::shared_ptr<Database> db, const std::string& where, const std::string& key, const std::string& value, QUERY_TYPE type = TYPE_EXIST, unsigned int index = 0);

    static void Move(User* user, std::shared_ptr<Database> db, const std::string& where, const std::string& entry, const std::string& value, const std::string& back);
};