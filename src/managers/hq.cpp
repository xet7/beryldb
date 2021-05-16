/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
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
#include "managers/hq.h"

void HQHelper::Search(User* user, std::shared_ptr<Database> database, const std::string& where, std::shared_ptr<HQuery> hquery)
{
       std::shared_ptr<search_query> query = std::make_shared<search_query>();

       query->database = database;
       query->user = user;
/*       query->limit = limit;
       query->offset = offset;
       query->qtype = type;
       query->select_query = where;
       query->int_keys = INT_KEYS;
       query->key = stripe(key);
*/
       Kernel->Store->Push(query);
}
