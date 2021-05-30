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

#include <tuple>

#include "beryl.h"
#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "managers/geo.h"

void GeoHelper::Add(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& latitude, const std::string& longitude)
{
       std::shared_ptr<geoadd_query> query = std::make_shared<geoadd_query>();
       Dispatcher::Check(query, user, MANAGER_TYPE_SINGLE);       
       
       query->database = database;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_GEO;
       query->key = entry;
       query->value = latitude;
       query->hesh = longitude;
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       Kernel->Store->Push(query);
}

void GeoHelper::Get(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry)
{
       std::shared_ptr<geoget_query> query = std::make_shared<geoget_query>();
       Dispatcher::Check(query, user, MANAGER_TYPE_SINGLE);       
       
       query->database = database;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_GEO;
       query->key = entry;
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       Kernel->Store->Push(query);
}

void GeoHelper::Del(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry)
{
       std::shared_ptr<geodel_query> query = std::make_shared<geodel_query>();
       
       Dispatcher::Check(query, user, MANAGER_TYPE_SINGLE);       
       
       query->database = database;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_GEO;
       query->key = entry;
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       Kernel->Store->Push(query);
}

void GeoHelper::Find(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<geofind_query> query = std::make_shared<geofind_query>();
       
       Dispatcher::Check(query, user, MANAGER_TYPE_LIST);       
       
       query->database = database;
       query->user = user;
       query->limit = limit;
       query->offset = offset;
       
       query->select_query = where;
       query->int_keys = INT_GEO;
       query->key = entry;
       Kernel->Store->Push(query);
}
