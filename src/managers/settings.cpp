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
#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "managers/databases.h"
#include "managers/lists.h"
#include "managers/settings.h"
#include "managers/maps.h"

bool STHelper::Set(const std::string& type, const std::string& key, const std::string& value)
{
        MapsHelper::Set(TABLE_SETTINGS, type, key, value);
        return true;
}

std::string STHelper::Get(const std::string& type, const std::string& key)
{
        BasicTuple tuple = MapsHelper::Get(TABLE_SETTINGS, type, key);
        const std::string response = std::get<1>(tuple);
        return response;
}

void STHelper::Delete(const std::string& type, const std::string& key)
{
        MapsHelper::Delete(TABLE_SETTINGS, type, key);
}
