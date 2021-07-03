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

#pragma once

#include "brldb/database.h"

class ExportAPI ExpireHelper
{
    public:

       static void List(std::shared_ptr<Database> db);

       static void Expire(User* user, const std::string& entry, unsigned int ttl);

       static void ExpireAT(User* user, const std::string& entry, unsigned int ttl);

       static void Persist(User* user, const std::string& entry);

       static void Persist(User* user, const std::string& entry, const std::string& select, std::shared_ptr<Database> db);

};
