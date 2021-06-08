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

void GeoHelper::Remove(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& value, const std::string& arg)
{
       std::shared_ptr<georemove_query> query = std::make_shared<georemove_query>();
       
       query->database = database;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_GEO;
       query->hesh = arg;
       query->key = entry;
       query->value = value;
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       Kernel->Store->Push(query);
}

void GeoHelper::GeoClose(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& value, signed int offset, signed int limit)
{
       std::shared_ptr<geoclose_query> query = std::make_shared<geoclose_query>();
       
       query->database = database;
       query->user = user;
       query->limit = limit;
       query->offset = offset;

       query->select_query = where;
       query->int_keys = INT_GEO;
       query->key = entry;
       query->value = value;
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       Kernel->Store->Push(query);
}

void GeoHelper::Calc(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& value)
{
       std::shared_ptr<geocalc_query> query = std::make_shared<geocalc_query>();
              
       
       query->database = database;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_GEO;
       query->key = entry;
       query->value = value;
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       Kernel->Store->Push(query);
}

void GeoHelper::Add(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& latitude, const std::string& longitude)
{
       std::shared_ptr<geoadd_query> query = std::make_shared<geoadd_query>();
              
       
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
       
              
       
       query->database = database;
       query->user = user;
       query->limit = limit;
       query->offset = offset;
       
       query->select_query = where;
       query->int_keys = INT_GEO;
       query->key = entry;
       Kernel->Store->Push(query);
}
