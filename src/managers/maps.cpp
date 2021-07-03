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
       query->value = to_bin(stripe(value));
       Kernel->Store->Push(query);
}

void MapsHelper::Get(User* user, const std::string& entry, const std::string& hesh)
{
       std::shared_ptr<hget_query> query = std::make_shared<hget_query>();
       Helpers::make_map(user, query, entry, hesh);
       Kernel->Store->Push(query);
}

void MapsHelper::Delete(User* user, const std::string& entry, const std::string& hesh)
{
       std::shared_ptr<hdel_query> query = std::make_shared<hdel_query>();
       Helpers::make_map(user, query, entry, hesh);
       Kernel->Store->Push(query);
}

void MapsHelper::HKeys(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<hkeys_query> query = std::make_shared<hkeys_query>();
       Helpers::make_map(user, query);
       query->key = entry;
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}


/*void MapsHelper::Move(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& hesh, const std::string& dest)
{
       std::shared_ptr<hmove_query> query = std::make_shared<hmove_query>();
              

       query->database = database;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_MAP;
       query->key = key;
       query->hesh = hesh;
       query->value = dest;

       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key) + ":" + to_bin(query->hesh);

       Kernel->Store->Push(query);
}


void MapsHelper::Count(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key)
{
       std::shared_ptr<hsearch_query> query = std::make_shared<hsearch_query>();
              

       query->database = database;
       query->user = user;
       query->key = key;
       query->int_keys = INT_MAP;
       query->select_query = where;
       query->limit = -1;
       query->offset = 0;

       query->qtype = TYPE_COUNT_RECORDS;

       query->format = query->int_keys + ":" + query->select_query + ":" + query->key;

       Kernel->Store->Push(query);

}

void //MapsHelper::HKeys(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, signed int offset, signed int limit)
{
       std::shared_ptr<hkeys_query> query = std::make_shared<hkeys_query>();
              
       
       query->database = database;
       query->user = user;
       query->key = key;
       query->offset = offset;
       query->limit = limit;
       query->int_keys = INT_MAP;
       query->select_query = where;

       Kernel->Store->Push(query);
}

void //MapsHelper::SearchHesh(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& hesh, signed int offset, signed int limit)
{
       std::shared_ptr<hsearch_hesh_query> query = std::make_shared<hsearch_hesh_query>();
              

       query->database = database;
       query->user = user;
       query->limit = limit;
       query->offset = offset;
       query->hesh = hesh;
       query->int_keys = INT_MAP;
       query->select_query = where;
       
       Kernel->Store->Push(query);
}

void //MapsHelper::Search(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, signed int offset, signed int limit)
{
       std::shared_ptr<hsearch_query> query = std::make_shared<hsearch_query>();
              
       
       query->database = database;
       query->user = user;
       query->limit = limit;
       query->offset = offset;
       query->key = key;
       query->int_keys = INT_MAP;
       query->select_query = where;

       Kernel->Store->Push(query);
}

void //MapsHelper::Get(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& hesh, QUERY_TYPE type)
{
       std::shared_ptr<hget_query> query = std::make_shared<hget_query>();
              

       query->database = database;
       query->user = user;
       query->select_query = where;
       query->hesh = hesh;
       query->int_keys = INT_MAP;
       query->key = key;
       query->qtype = type;
       
       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key) + ":" + to_bin(query->hesh);
       
       Kernel->Store->Push(query);
}

void //MapsHelper::Set(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& hesh, const std::string& value)
{
       std::shared_ptr<hset_query> query = std::make_shared<hset_query>();
              

       query->database = database;
       query->user = user;
       query->select_query = where;
       query->value = to_bin(stripe(value));
       query->hesh = hesh;
       query->int_keys = INT_MAP;
       query->key = entry;

       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key) + ":" + to_bin(query->hesh);

       Kernel->Store->Push(query);
}

void //MapsHelper::DeleteAll(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry)
{
       std::shared_ptr<hdel_all_query> query = std::make_shared<hdel_all_query>();
              

       query->database = database;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_MAP;
       query->key = entry;

       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key);

       Kernel->Store->Push(query);
}

void //MapsHelper::Delete(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& hesh)
{
       std::shared_ptr<hdel_query> query = std::make_shared<hdel_query>();
              

       query->database = database;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_MAP;
       query->key = entry;
       query->hesh = hesh;

       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key) + ":" + to_bin(query->hesh);

       Kernel->Store->Push(query);
}

BasicTuple //MapsHelper::Get(const std::string& where, const std::string& key, const std::string& hesh)
{
       hget_query query;
       
       query.database = Kernel->Core->DB;

       query.select_query = where;
       query.key = key;
       query.int_keys = INT_MAP;

       
       query.hesh = hesh;
       query.format = query.int_keys + ":" + query.select_query + ":" + to_bin(query.key) + ":" + to_bin(query.hesh);
       
       query.Run();
       return std::make_tuple(query.access, query.response);
}

DBL_CODE //MapsHelper::Set(const std::string& where, const std::string& entry, const std::string& hesh, const std::string& value)
{
       std::shared_ptr<hset_query> query = std::make_shared<hset_query>();

       query->database = Kernel->Core->DB;
       query->select_query = where;
       query->value = to_bin(value);
       query->hesh = hesh;
       query->int_keys = INT_MAP;
       query->key = entry;

       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key) + ":" + to_bin(query->hesh);
       query->Run();
       
       return DBL_MANAGER_OK;
}

DBL_CODE //MapsHelper::Delete(const std::string& where, const std::string& entry, const std::string& value)
{
       std::shared_ptr<hdel_query> query = std::make_shared<hdel_query>();

       query->database = Kernel->Core->DB;
       query->select_query = where;
       query->int_keys = INT_MAP;
       query->key = entry;
       query->hesh = value;

       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key) + ":" + to_bin(query->hesh);
       query->Run();
       
       return DBL_MANAGER_OK;
}

DBL_CODE //MapsHelper::DeleteAll(const std::string& where, const std::string& entry)
{
       std::shared_ptr<hdel_all_query> query = std::make_shared<hdel_all_query>();

       query->database = Kernel->Core->DB;
       query->select_query = where;
       query->int_keys = INT_MAP;
       query->key = entry;

       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key);
       query->Run();

       return DBL_MANAGER_OK;
}


VectorTuple //MapsHelper::Search(QUERY_TYPE type, const std::string& where, const std::string& key, signed int offset, signed int limit)
{
       std::shared_ptr<hsearch_query> query = std::make_shared<hsearch_query>();

       query->database = Kernel->Core->DB;
       query->limit = limit;
       query->offset = offset;
       query->key = key;
       query->int_keys = INT_MAP;
       query->select_query = where;
       query->core = true;

       query->Run();

       return std::make_tuple(query->access, query->VecData);
}

VectorTuple //MapsHelper::HKeys(const std::string& where, const std::string& key)
{
       std::shared_ptr<hkeys_query> query = std::make_shared<hkeys_query>();

       query->database = Kernel->Core->DB;
       
       query->key = key;
       query->offset = 0;
       query->limit = -1;
       query->int_keys = INT_MAP;
       query->select_query = where;
       query->core = true;

       query->Run();

       return std::make_tuple(query->access, query->VecData);
}

MMapTuple //MapsHelper::SearchHesh(const std::string& where, const std::string& hesh, signed int offset, signed int limit)
{
       std::shared_ptr<hsearch_hesh_query> query = std::make_shared<hsearch_hesh_query>();
       
       query->database = Kernel->Core->DB;
       query->hesh = hesh;
       query->limit = limit;
       query->offset = offset;
       
       query->int_keys = INT_MAP;
       query->select_query = where;
       query->core = true;

       query->Run();
       
       return std::make_tuple(query->access, query->mlist);
}
*/