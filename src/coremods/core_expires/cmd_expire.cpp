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
#include "maker.h"
#include "converter.h"
#include "core_expires.h"

CommandExpire::CommandExpire(Module* Creator) : Command(Creator, "EXPIRE", 2, 2)
{
         group = 'e';
         syntax = "<key> <seconds>";
}

COMMAND_RESULT CommandExpire::Handle(User* user, const Params& parameters) 
{       
          const std::string& key = parameters[0];
          const std::string& seconds = parameters[1];
          
          if (!CheckValidPos(user, seconds))
          {
                return FAILED;
          }
                  
          ExpireHelper::Expire(user, key, convto_num<unsigned int>(seconds));
          return SUCCESS;
}

CommandDBEReset::CommandDBEReset(Module* Creator) : Command(Creator, "ERESET", 1, 1)
{
          group = 'e';
          syntax = "<database>";
}

COMMAND_RESULT CommandDBEReset::Handle(User* user, const Params& parameters) 
{       
          const std::string& dbname = parameters[0];
    
          std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);

          if (!database)
          {
                user->SendProtocol(ERR_QUERY, PROCESS_NULL);
                return FAILED;
          }
          
          if (database->IsClosing())
          {
                user->SendProtocol(ERR_QUERY, DATABASE_BUSY);
                return FAILED;
          }
        
          Kernel->Store->Expires->DatabaseReset(database->GetName());
          return SUCCESS;
}
