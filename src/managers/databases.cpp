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
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "managers/databases.h"
#include "managers/lists.h"
#include "managers/maps.h"
#include "helpers.h"

bool DBHelper::FlushDB(std::shared_ptr<Database> database, bool notify)
{
        Kernel->Store->Flusher->Pause();
        bool result = database->FlushDB();
        Kernel->Store->Flusher->Resume();

        if (notify)
        {
             bprint(DONE, "Database flushed.");
        }
        
        return result;
}

void DBHelper::Type(User* user, const std::string& key)
{
       std::shared_ptr<type_query> query = std::make_shared<type_query>();
       Helpers::make_query(user, query);

       query->key = key;
       query->format = to_bin(query->key) + ":" + query->select_query + ":" + query->base_request;
       Kernel->Store->Push(query);
}

void DBHelper::DBSize(User* user)
{
       std::shared_ptr<dbsize_query> query = std::make_shared<dbsize_query>();
       Helpers::make_query(user, query);
       Kernel->Store->Push(query);
}

/*

void DBHelper::SwapDB(User* user, std::shared_ptr<Database> database, const std::string& db1, const std::string& db2)
{
       std::shared_ptr<swapdb_query> query = std::make_shared<swapdb_query>();

       query->database = database;
       query->user = user;
       
       query->key = db1;
       query->value = db2;

       Kernel->Store->Push(query);
}

void DBHelper::SFlush(User* user, std::shared_ptr<Database> database, const std::string& select)
{
       std::shared_ptr<sflush_query> query = std::make_shared<sflush_query>();

       query->database = database;
       query->user = user;
       query->value = select;
       
       Kernel->Store->Push(query);
}

bool DBHelper::Add(const std::string& name, const std::string& path)
{
        /* default database add */
 
        //MapsHelper::Set(TABLE_DBS, name, "name", name);
        //MapsHelper::Set(TABLE_DBS, name, "path", path);
  /*      return //MapsHelper::Set(TABLE_DBS, name, "created", convto_string(Kernel->Now()));
}

std::string DBHelper::Find(const std::string& name, const std::string& key)
{
        BasicTuple tuple = //MapsHelper::Get(TABLE_DBS, name, key);
        std::string lst = std::get<1>(tuple);
        return lst;
}
*/