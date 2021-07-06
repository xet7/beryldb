
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

class ExportAPI MMapsHelper
{
    public:

       static void Get(User* user, const std::string& entry, signed int offset, signed int limit);
       
       static void Iter(User* user, const std::string& entry,  signed int offset, signed int limit);
       
       static void Count(User* user, const std::string& entry);

       static void Set(User* user, const std::string& entry, const std::string& hesh, const std::string& value);

       static void Del(User* user, const std::string& entry, const std::string& hesh);
       
       static void Seek(User* user, const std::string& entry, const std::string& hesh, signed int offset, signed int limit);
       
       
};