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

class Externalize KeyHelper
{
    public:

        static void Append(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& value);

        static void Touch(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& value = "", QUERY_TYPE type = TYPE_NONE);

        static void GetRange(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, int from, int to);

        static void AdvGet(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, QUERY_TYPE type = TYPE_NONE);

        static void AdvSet(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& value, QUERY_TYPE type = TYPE_NONE);

        static void Count(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& customreply = "");

        static void Find(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, signed int offset, signed int limit, QUERY_TYPE type = TYPE_NONE);

        static void Expire(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, QUERY_TYPE type, unsigned int usig, std::string value = "");

        static void Get(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, QUERY_TYPE type = TYPE_NONE);

        static void Operation(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, OP_TYPE type,  const std::string& oper = "");

        static void Delete(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, bool quiet = false);
        
        static void Set(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& value, const std::string& custom = "", QUERY_TYPE type = TYPE_NONE);

        /* Moves an entry to another select. */
        
        static void Move(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& new_select);

};
