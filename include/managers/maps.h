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

class ExportAPI CMapsHelper
{
    public:
       
       static MapData Set(const std::string& entry, const std::string& hesh, const std::string& value);
       
       static MapData Get(const std::string& key, const std::string& hesh);

       static MapData Del(const std::string& key, const std::string& hesh);

       static MapData HList(const std::string& entry);
       
       static MapData Erase(const std::string& entry);
};