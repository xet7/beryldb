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
#include "managers/keys.h"
#include "managers/globals.h"
#include "helpers.h"
#include "extras.h"

void GlobalHelper::Clone(User* user, const std::string& entry, const std::string& dest)
{
       std::shared_ptr<clone_query> query = std::make_shared<clone_query>();
       Helpers::make_query(user, query, entry);
       query->value = dest;
       Kernel->Store->Push(query);
}

void GlobalHelper::Copy(User* user, const std::string& entry, const std::string& dest)
{
       std::shared_ptr<copy_query> query = std::make_shared<copy_query>();
       Helpers::make_query(user, query, entry);
       query->value = dest;
       Kernel->Store->Push(query);
}

void GlobalHelper::Delete(User* user, const std::string& entry)
{
       std::shared_ptr<del_query> query = std::make_shared<del_query>();
       Helpers::make_query(user, query, entry);
       
       Kernel->Store->Push(query);
}

void GlobalHelper::Rename(User* user, const std::string& entry, const std::string& dest)
{
       std::shared_ptr<rename_query> query = std::make_shared<rename_query>();
       Helpers::make_query(user, query, entry);
       query->value = dest;
       Kernel->Store->Push(query);
}

void GlobalHelper::RenameNX(User* user, const std::string& entry, const std::string& dest)
{
       std::shared_ptr<renamenx_query> query = std::make_shared<renamenx_query>();
       Helpers::make_query(user, query, entry);
       query->value = dest;
       Kernel->Store->Push(query);
}


void GlobalHelper::Move(User* user, const std::string& entry, const std::string& dest)
{
       std::shared_ptr<move_query> query = std::make_shared<move_query>();
       Helpers::make_query(user, query, entry);
       query->value = dest;
       Kernel->Store->Push(query);
}

void GlobalHelper::Exists(User* user, const std::string& entry)
{
       std::shared_ptr<exists_query> query = std::make_shared<exists_query>();
       Helpers::make_query(user, query, entry);
       Kernel->Store->Push(query);
}

void GlobalHelper::ExpireDelete(std::shared_ptr<Database> database, const std::string& where, const std::string& key)
{
       std::shared_ptr<del_query> query = std::make_shared<del_query>();

       query->user = Kernel->Clients->Global;
       query->database = database;
       query->select_query = where;
       query->flags = QUERY_FLAGS_QUIET;
       query->key = key;
       
       Kernel->Store->Push(query);
}

void GlobalHelper::FutureExecute(std::shared_ptr<Database> database, const std::string& where, const std::string& key)
{
       std::shared_ptr<future_exec_query> query = std::make_shared<future_exec_query>();

       query->user = Kernel->Clients->Global;
       query->database = database;
       query->select_query = where;
       query->flags = QUERY_FLAGS_QUIET;
       query->key = key;
       Kernel->Store->Push(query);
}

void GlobalHelper::UserFutureExecute(User* user, const std::string& key)
{
       std::shared_ptr<future_exec_query> query = std::make_shared<future_exec_query>();
       Helpers::make_query(user, query, key);
       Kernel->Store->Push(query);
}

void GlobalHelper::FutureCancel(User* user, const std::string& key)
{
       std::shared_ptr<future_del_query> query = std::make_shared<future_del_query>();
       Helpers::make_query(user, query, key);
       Kernel->Store->Push(query);
}

void GlobalHelper::FutureGlobalCancel(std::shared_ptr<Database> database, const std::string& where, const std::string& key)
{
       std::shared_ptr<future_del_query> query = std::make_shared<future_del_query>();

       query->user = Kernel->Clients->Global;
       query->database = database;
       query->select_query = where;
       query->flags = QUERY_FLAGS_QUIET;
       query->key = key;
       Kernel->Store->Push(query);
}

void GlobalHelper::Diff(User* user, const std::string& key, const std::string& value, signed int offset, signed int limit)
{
       std::shared_ptr<diff_query> query = std::make_shared<diff_query>();
       Helpers::make_query(user, query, key);
       query->value = value;
       query->limit = limit;
       query->offset = offset;
       Kernel->Store->Push(query);
}

