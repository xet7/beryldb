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

class ExportAPI VectorHelper
{
    public:

       static void PushNX(User* user, const std::string& entry, const std::string& value);

       static void Push(User* user, const std::string& entry, const std::string& value);
       
       static void Get(User* user, const std::string& entry, signed int offset, signed int limit);

       static void Count(User* user, const std::string& entry);
       
       static void Index(User* user, const std::string& key, const std::string& entry);
       
       static void Resize(User* user, const std::string& entry, const std::string& value);

       static void Keys(User* user, const std::string& entry, signed int offset, signed int limit);

       static void Exist(User* user, const std::string& entry, const std::string& value);
       
       static void PopFront(User* user, const std::string& entry);
       
       static void PopBack(User* user, const std::string& entry);

       static void Del(User* user, const std::string& entry, const std::string& value);
       
       static void EraseFrom(User* user, const std::string& entry, const std::string& value);

       static void Reverse(User* user, const std::string& entry);
       
       static void Repeats(User* user, const std::string& entry, const std::string& value);
       
       static void Sort(User* user, const std::string& entry);

       static void Avg(User* user, const std::string& entry);

       static void High(User* user, const std::string& entry);

       static void Low(User* user, const std::string& entry);

       static void Sum(User* user, const std::string& entry);
       
       static void Find(User* user, const std::string& entry, const std::string& value, signed int offset, signed int limit);
       
};