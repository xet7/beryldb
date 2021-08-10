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

#include "beryl.h"
#include "cstruct.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "managers/databases.h"
#include "managers/maps.h"
#include "managers/settings.h"

bool STHelper::Set(const std::string& type, const std::string& key, const std::string& value)
{
        CMapsHelper::Set(type, key, value);
        return true;
}

std::string STHelper::Get(const std::string& type, const std::string& key)
{
        return CMapsHelper::Get(type, key).response;
}

Args STHelper::HKeys(const std::string& key)
{
       return CMapsHelper::HKeys(key).list;
}

void STHelper::Delete(const std::string& type, const std::string& key)
{
        CMapsHelper::Del(type, key);
}

void STHelper::Erase(const std::string& type)
{
        CMapsHelper::Erase(type);
}
