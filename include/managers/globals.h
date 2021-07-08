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

#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "brldb/expires.h"
#include "helpers.h"

class ExportAPI GlobalHelper
{
    public:

      static void Clone(User* user, const std::string& entry, const std::string& dest);

      static void Copy(User* user, const std::string& entry, const std::string& dest);
    
      static void Rename(User* user, const std::string& entry, const std::string& dest);

      static void Move(User* user, const std::string& entry, const std::string& dest);

      static void Delete(User* user, const std::string& entry);
      
      static void ExpireDelete(std::shared_ptr<Database> database, const std::string& where, const std::string& key);

      static void Exists(User* user, const std::string& entry);
      
      static void FutureExecute(std::shared_ptr<Database> database, const std::string& where, const std::string& key);

      static void UserFutureExecute(User* user, const std::string& key);

      static void FutureCancel(User* user, const std::string& key);

      static void FutureGlobalCancel(std::shared_ptr<Database> database, const std::string& where, const std::string& key);
      
};