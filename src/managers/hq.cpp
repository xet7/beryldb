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
#include "managers/hq.h"

void HQHelper::HSend(User* user, std::shared_ptr<Database> database, const std::string& where, std::shared_ptr<HQuery> hquery)
{
       std::shared_ptr<hquery_query> query = std::make_shared<hquery_query>();
       
       std::shared_ptr<HQuery> newptr = hquery;

       hquery = nullptr;
       user->hquery.reset();
       user->hquery = nullptr;
       
       query->database = database;
       query->user = user;
       query->hquery = newptr;
       
       Kernel->Store->Push(query);
}
