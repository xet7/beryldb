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
#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "helpers.h"
#include "managers/maps.h"


void MapsHelper::Exists(User* user, const std::string& entry, const std::string& hesh)
{
       std::shared_ptr<hexists_query> query = std::make_shared<hexists_query>();
       Helpers::make_map(user, query, entry, hesh);
       Kernel->Store->Push(query);
}

void MapsHelper::Set(User* user, const std::string& entry, const std::string& hesh, const std::string& value)
{
       std::shared_ptr<hset_query> query = std::make_shared<hset_query>();
       Helpers::make_map(user, query, entry, hesh);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void MapsHelper::SetNX(User* user, const std::string& entry, const std::string& hesh, const std::string& value)
{
       std::shared_ptr<hsetnx_query> query = std::make_shared<hsetnx_query>();
       Helpers::make_map(user, query, entry, hesh);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void MapsHelper::WDelete(User* user, const std::string& key, const std::string& hesh)
{
       std::shared_ptr<hwdel_query> query = std::make_shared<hwdel_query>();
       Helpers::make_map(user, query, key, hesh);
       Kernel->Store->Push(query);
}

void MapsHelper::Get(User* user, const std::string& entry, const std::string& hesh)
{
       std::shared_ptr<hget_query> query = std::make_shared<hget_query>();
       Helpers::make_map(user, query, entry, hesh);
       Kernel->Store->Push(query);
}

void MapsHelper::Strlen(User* user, const std::string& entry, const std::string& hesh)
{
       std::shared_ptr<hstrlen_query> query = std::make_shared<hstrlen_query>();
       Helpers::make_map(user, query, entry, hesh);
       Kernel->Store->Push(query);
}

void MapsHelper::Delete(User* user, const std::string& entry, const std::string& hesh)
{
       std::shared_ptr<hdel_query> query = std::make_shared<hdel_query>();
       Helpers::make_map(user, query, entry, hesh);
       Kernel->Store->Push(query);
}

void MapsHelper::Find(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<hfind_query> query = std::make_shared<hfind_query>();
       Helpers::make_map(user, query, "", "", true);
       query->key = entry;
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void MapsHelper::List(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<hlist_query> query = std::make_shared<hlist_query>();
       Helpers::make_map(user, query);
       query->key = entry;
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void MapsHelper::Count(User* user, const std::string& entry)
{
       std::shared_ptr<hlist_query> query = std::make_shared<hlist_query>();
       Helpers::make_map(user, query);
       
       query->flags = QUERY_FLAGS_COUNT;
       query->key = entry;
       Kernel->Store->Push(query);
}

void MapsHelper::Vals(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<hvals_query> query = std::make_shared<hvals_query>();
       Helpers::make_map(user, query, entry);
       
       query->key = entry;
       query->offset = offset;
       query->limit = limit;
       
       Kernel->Store->Push(query);
}

void MapsHelper::GetAll(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<hgetall_query> query = std::make_shared<hgetall_query>();
       Helpers::make_map(user, query, entry);
       
       query->key = entry;
       query->offset = offset;
       query->limit = limit;
       
       Kernel->Store->Push(query);
}


