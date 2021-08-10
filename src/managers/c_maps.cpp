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
#include "extras.h"
#include "helpers.h"

#include "managers/maps.h"

MapData CMapsHelper::Set(const std::string& entry, const std::string& hesh, const std::string& value)
{
       std::shared_ptr<hset_query> query = std::make_shared<hset_query>();
       Helpers::make_cmap(query, entry, hesh);
       
       query->value = value;
       query->Prepare();
       
       return MapData(DBL_MANAGER_OK);
}


MapData CMapsHelper::Get(const std::string& key, const std::string& hesh)
{
       std::shared_ptr<hget_query> query = std::make_shared<hget_query>();
       Helpers::make_cmap(query, key, hesh);

       query->Prepare();

       return MapData(query->access, query->response);
}

MapData CMapsHelper::Del(const std::string& key, const std::string& hesh)
{
       std::shared_ptr<hdel_query> query = std::make_shared<hdel_query>();
       Helpers::make_cmap(query, key, hesh);

       query->Prepare();

       return MapData(query->access);
}

MapData CMapsHelper::HKeys(const std::string& entry)
{
       std::shared_ptr<hlist_query> query = std::make_shared<hlist_query>();
       Helpers::make_cmap(query, entry);
       
       query->key = entry;
       query->Prepare();

       return MapData(query->access, query->VecData);
}

MapData CMapsHelper::Erase(const std::string& entry)
{
       std::shared_ptr<del_query> query = std::make_shared<del_query>();
       Helpers::make_cmap(query, entry);
       
       query->key = entry;
       query->Prepare();
       
       return MapData(query->access);
}
