/*
 * BerylDB - A modular database.
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

void DBHelper::DBSize(User* user, std::shared_ptr<Database> database)
{
       std::shared_ptr<dbsize_query> query = std::make_shared<dbsize_query>();

       query->database = database;
       query->user = user;
       
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
 
        MapsHelper::Set(TABLE_DBS, name, "name", name);
        MapsHelper::Set(TABLE_DBS, name, "path", path);
        return MapsHelper::Set(TABLE_DBS, name, "created", convto_string(Kernel->Now()));
}


std::string DBHelper::Find(const std::string& name, const std::string& key)
{
        BasicTuple tuple = MapsHelper::Get(TABLE_DBS, name, key);
        std::string lst = std::get<1>(tuple);
        return lst;
}

bool DBHelper::FlushDB(bool notify)
{
        Kernel->Store->Flusher->Pause();
        bool result = Kernel->Store->Default->FlushDB();
        MapsHelper::DeleteAll("lcounter", "id");
        Kernel->Store->Flusher->Resume();

        if (notify)
        {
             bprint(DONE, "Database flushed.");
        }
        
        return result;
}