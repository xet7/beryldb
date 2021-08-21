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
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "managers/expires.h"
#include "helpers.h"
#include "extras.h"

void ExpireHelper::ListFutures(std::shared_ptr<Database> db, bool last)
{
       std::shared_ptr<future_list_query> query = std::make_shared<future_list_query>();
       
       query->database 	=  db;
       query->user 	=  Kernel->Clients->Global;
       query->flags	=  QUERY_FLAGS_GLOBAL;
       
       if (last)
       {
               query->id = 1000;
       }
       
       Kernel->Store->Push(query);
}


void ExpireHelper::List(std::shared_ptr<Database> db)
{
       std::shared_ptr<expire_list_query> query = std::make_shared<expire_list_query>();
       
       query->database = db;
       query->user = Kernel->Clients->Global;
       Kernel->Store->Push(query);
}

void ExpireHelper::Future(User* user, const std::string& entry, unsigned int ttl, const std::string& value)
{
       std::shared_ptr<future_query> query = std::make_shared<future_query>();
       Helpers::make_query(user, query, entry);

       query->value = stripe(value);
       query->id = Kernel->Now() + ttl;
       Kernel->Store->Push(query);
}

void ExpireHelper::FutureAT(User* user, const std::string& entry, unsigned int ttl, const std::string& value)
{
       std::shared_ptr<future_query> query = std::make_shared<future_query>();
       Helpers::make_query(user, query, entry);

       query->value = stripe(value);
       query->id = ttl;
       Kernel->Store->Push(query);
}

void ExpireHelper::Expire(User* user, const std::string& entry, unsigned int ttl)
{
       std::shared_ptr<expire_query> query = std::make_shared<expire_query>();
       Helpers::make_query(user, query, entry);

       query->id = Kernel->Now() + ttl;
       Kernel->Store->Push(query);
}

void ExpireHelper::Setex(User* user, unsigned int exp_usig, const std::string& key, const std::string& value)
{
       std::shared_ptr<setex_query> query = std::make_shared<setex_query>();
       Helpers::make_query(user, query, key);

       query->value = stripe(value);
       query->id = Kernel->Now() + exp_usig;
       Kernel->Store->Push(query);
}

void ExpireHelper::ExpireAT(User* user, const std::string& entry, unsigned int ttl)
{
       std::shared_ptr<expireat_query> query = std::make_shared<expireat_query>();
       Helpers::make_query(user, query, entry);

       query->id = ttl;
       Kernel->Store->Push(query);
}

void ExpireHelper::Persist(User* user, const std::string& entry, const std::string& select, std::shared_ptr<Database> db)
{
       std::shared_ptr<expire_del_query> query = std::make_shared<expire_del_query>();
       
       query->database = db;
       query->flags = QUERY_FLAGS_QUIET;
       query->select_query = select;
       query->user = user;
       query->key = entry;

       Kernel->Store->Push(query);
}

void ExpireHelper::QuickPersist(User* user, const std::string& key)
{
       std::shared_ptr<expire_del_query> query = std::make_shared<expire_del_query>();
       Helpers::make_query(user, query, key);

       Kernel->Store->Push(query);
}
