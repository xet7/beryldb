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

class ExportAPI GeoHelper
{
    public:

       static void Remove(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& value, const std::string& arg);

       static void GeoClose(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& value, signed int offset, signed int limit);
    
       static void Calc(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& value);

       static void Add(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& latitude, const std::string& longitude);

       static void Get(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry);

       static void Del(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry);

       static void Find(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, signed int offset, signed int limit);
};