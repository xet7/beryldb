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
#include "helpers.h"

void GeoHelper::Add(User* user, const std::string& key, const std::string& latitude, const std::string& longitude)
{
       std::shared_ptr<geoadd_query> query = std::make_shared<geoadd_query>();
       Helpers::make_geo_query(user, query, key);
       query->value = latitude;
       query->hesh = longitude;
       Kernel->Store->Push(query);
}

void GeoHelper::Get(User* user, const std::string& key)
{
       std::shared_ptr<geoget_query> query = std::make_shared<geoget_query>();
       Helpers::make_geo_query(user, query, key);
       Kernel->Store->Push(query);
}

void GeoHelper::Find(User* user, const std::string& key, signed int offset, signed int limit)
{
       std::shared_ptr<gkeys_query> query = std::make_shared<gkeys_query>();
       Helpers::make_geo_query(user, query, key);
       
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void GeoHelper::Calc(User* user, const std::string& key, const std::string& key2)
{
       std::shared_ptr<geocalc_query> query = std::make_shared<geocalc_query>();
       Helpers::make_geo_query(user, query, key);
       
       query->value = key2;
       Kernel->Store->Push(query);
}

void GeoHelper::Remove(User* user, const std::string& key, const std::string& key2)
{
       std::shared_ptr<georem_query> query = std::make_shared<georem_query>();
       Helpers::make_geo_query(user, query, key);

       query->value = key2;
       Kernel->Store->Push(query);
}

void GeoHelper::Distance(User* user, const std::string& key, const std::string& key2, signed int offset, signed int limit)
{
       std::shared_ptr<geodistance_query> query = std::make_shared<geodistance_query>();
       Helpers::make_geo_query(user, query, key);
       
       query->value = key2;
       query->offset = offset;
       query->limit = limit;
       
       Kernel->Store->Push(query);
}
