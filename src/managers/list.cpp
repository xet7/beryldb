/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
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
#include "managers/lists.h"
#include "managers/maps.h"

VectorTuple ListHelper::Get(const std::string& where, const std::string& key)
{
       std::shared_ptr<lget_query> query = std::make_shared<lget_query>();

       query->database = Kernel->Core->DB;
       query->offset = 0;
       query->limit = -1;
       query->int_keys = INT_LIST;
       query->select_query = where;
       query->key = key;
       query->core = true;

       query->Run();
       

       return std::make_tuple(query->access, query->VecData);
}

DBL_CODE ListHelper::Add(const std::string& where, const std::string& entry, const std::string& value)
{
       std::shared_ptr<lpush_query> query = std::make_shared<lpush_query>();
       query->database = Kernel->Core->DB;
       query->select_query = where;
       query->int_keys = INT_LIST;
       query->key = entry;
       query->value = to_bin(value);
       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key);

       BasicTuple tuple = MapsHelper::Get("lcounter", "id", query->format);
       std::string lcounter = std::get<1>(tuple);

       unsigned int id_num = 0;
       id_num = convto_num<unsigned int>(lcounter);
       id_num++;

       query->id = id_num;
       query->Run();

       MapsHelper::Set("lcounter", "id", query->format, convto_string(id_num));


       return DBL_MANAGER_OK;
}

DBL_CODE ListHelper::Delete(const std::string& where, const std::string& entry, const std::string& value, bool onlyfirst)
{
       std::shared_ptr<lpop_query> query = std::make_shared<lpop_query>();

       query->database = Kernel->Core->DB;
       query->select_query = where;
       query->int_keys = INT_LIST;
       query->key = to_bin(entry);
       query->value = to_bin(value);
       query->all = onlyfirst;

       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key);
       
       query->Run();
       
       return DBL_MANAGER_OK;
}

/* User helpers */

void ListHelper::Get(User* user, std::shared_ptr<Database> db, const std::string& where, const std::string& key, signed int offset, signed int limit, QUERY_TYPE type)
{
       std::shared_ptr<lget_query> query = std::make_shared<lget_query>();

       query->database = db;
       query->user = user;
       query->offset = offset;
       query->limit = limit;
       query->int_keys = INT_LIST;
       query->select_query = where;
       query->key = key;
       query->qtype = type;
       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key);

       Kernel->Store->Push(query);
}

void ListHelper::Exist(User* user, std::shared_ptr<Database> db, const std::string& where, const std::string& key, const std::string& value, QUERY_TYPE type, unsigned int index)
{
       std::shared_ptr<lget_query> query = std::make_shared<lget_query>();

       query->database = db;
       query->user = user;
       query->value = to_bin(stripe(value));
       query->int_keys = INT_LIST;
       query->id = index;
       query->select_query = where;
       query->key = key;
       query->qtype = type;

       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key);

       Kernel->Store->Push(query);
}

void ListHelper::Add(User* user, std::shared_ptr<Database> db, const std::string& where, const std::string& entry, const std::string& value)
{
       std::shared_ptr<lpush_query> query = std::make_shared<lpush_query>();
       query->database = db;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_LIST;
       query->key = entry;
       query->value = to_bin(stripe(value));
       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key);

       BasicTuple tuple = MapsHelper::Get("lcounter", "id", query->format);
       std::string lcounter = std::get<1>(tuple);

       unsigned int id_num = 0;
       id_num = convto_num<unsigned int>(lcounter);
       id_num++;

       query->id = id_num;
       Kernel->Store->Push(query);

       MapsHelper::Set("lcounter", "id", query->format, convto_string(id_num));
}

void ListHelper::Move(User* user, std::shared_ptr<Database> db, const std::string& where, const std::string& select, const std::string& key, const std::string& value)
{
       std::shared_ptr<lmove_query> query = std::make_shared<lmove_query>();
       query->database = db;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_LIST;
       query->key = to_bin(key);
       query->value = to_bin(stripe(value));
       query->hesh = select;
       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key);
       Kernel->Store->Push(query);
}

void ListHelper::Delete(User* user, std::shared_ptr<Database> db, const std::string& where, const std::string& entry,  const std::string& value, bool onlyfirst)
{
       std::shared_ptr<lpop_query> query = std::make_shared<lpop_query>();

       query->database = db;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_LIST;
       query->key = to_bin(entry);
       query->value = to_bin(stripe(value));
       query->all = onlyfirst;

       query->format = query->int_keys + ":" + query->select_query + ":" + to_bin(query->key);

       Kernel->Store->Push(query);
}
