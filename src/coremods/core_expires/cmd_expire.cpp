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
#include "brldb/dbflush.h"
#include "brldb/expires.h"
#include "managers/expires.h"
#include "engine.h"
#include "converter.h"
#include "core_expires.h"

CommandExpire::CommandExpire(Module* Creator) : Command(Creator, "EXPIRE", 2, 2)
{
          syntax = "<key> <seconds>";
}

COMMAND_RESULT CommandExpire::Handle(User* user, const Params& parameters) 
{       
          const std::string& key = parameters[0];
          const std::string& exp_str = parameters[1];
          
          if (!is_number(exp_str))
          {
                 user->SendProtocol(ERR_EXPIRE, MUST_BE_NUMERIC);
                 return FAILED;
          }
          
          if (!is_positive_number(exp_str))
          {
                 user->SendProtocol(ERR_EXPIRE, MUST_BE_POSIT);
                 return FAILED;
          }
                  
          unsigned int exp_usig = convto_num<unsigned int>(exp_str);
          ExpireHelper::Expire(user, key, exp_usig);
          return SUCCESS;
}

CommandDBEReset::CommandDBEReset(Module* Creator) : Command(Creator, "ERESET", 1, 1)
{
          syntax = "<database>";
}

COMMAND_RESULT CommandDBEReset::Handle(User* user, const Params& parameters) 
{       
          const std::string& dbname = parameters[0];
    
          std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);

          if (!database)
          {
                user->SendProtocol(ERR_DB_NOT_FOUND, PROCESS_NULL);
                return FAILED;
          }
          
          if (database->IsClosing())
          {
                user->SendProtocol(ERR_DB_BUSY, DATABASE_BUSY);
                return FAILED;
          }

          Kernel->Store->Expires->DatabaseReset(database->GetName());
          return SUCCESS;
}
