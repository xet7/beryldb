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

class ExportAPI ListHelper
{
    public:

        static void Push(User* user, const std::string& entry, const std::string& hesh);
        
        static void Get(User* user, const std::string& entry, signed int offset, signed int limit);

        static void PopFront(User* user, const std::string& entry);
        
        static void PopBack(User* user, const std::string& entry);

        static void Count(User* user, const std::string& entry);
        
        static void PopAll(User* user, const std::string& entry, const std::string& value);
        
        static void Reverse(User* user, const std::string& entry);
        
        static void Find(User* user, const std::string& entry, const std::string& value, signed int offset, signed int limit);
        
        static void Keys(User* user, const std::string& entry, signed int offset, signed int limit);

        static void Resize(User* user, const std::string& entry, const std::string& value);
        
        static void Sort(User* user, const std::string& entry);

        static void Index(User* user, const std::string& key, const std::string& entry);
        
        static void Exist(User* user, const std::string& entry, const std::string& value);
        
        static void Del(User* user, const std::string& entry, const std::string& value);

        static void Repeats(User* user, const std::string& entry, const std::string& value);
        
        static void Rop(User* user, const std::string& entry);
        
        static void FRop(User* user, const std::string& entry);
        
        
};