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
#include "managers/expires.h"
#include "helpers.h"
#include "extras.h"

void ExpireHelper::List(std::shared_ptr<Database> db)
{
       std::shared_ptr<expire_list_query> query = std::make_shared<expire_list_query>();
       
       query->database = db;
       query->user = Kernel->Clients->Global;
       Kernel->Store->Push(query);
       
       bprint(DONE, "Loading expires: %s", db->GetName().c_str());
}

void ExpireHelper::Expire(User* user, const std::string& entry, unsigned int ttl)
{
       std::shared_ptr<expire_query> query = std::make_shared<expire_query>();
       Helpers::make_query(user, query, entry);

       query->dest = to_bin(query->key) + ":" + query->select_query + ":" + query->base_request;

       query->id = Kernel->Now() + ttl;
       Kernel->Store->Push(query);
}

void ExpireHelper::ExpireAT(User* user, const std::string& entry, unsigned int ttl)
{
       std::shared_ptr<expireat_query> query = std::make_shared<expireat_query>();
       Helpers::make_query(user, query, entry);

       query->id = ttl;
       query->dest = to_bin(query->key) + ":" + query->select_query + ":" + query->base_request;

       Kernel->Store->Push(query);
}

void ExpireHelper::Persist(User* user, const std::string& entry)
{
       std::shared_ptr<expire_del_query> query = std::make_shared<expire_del_query>();
       Helpers::make_query(user, query, entry);

       query->dest = to_bin(query->key) + ":" + query->select_query + ":" + query->base_request;
       Kernel->Store->Push(query);
}

void ExpireHelper::Persist(User* user, const std::string& entry, const std::string& select, std::shared_ptr<Database> db)
{
       std::shared_ptr<expire_del_query> query = std::make_shared<expire_del_query>();
       
       query->database = db;
       query->quiet = true;
       query->select_query = select;
       query->user = user;
       query->key = entry;
       query->dest = to_bin(query->key) + ":" + query->select_query + ":" + query->base_request;

       Kernel->Store->Push(query);
}

