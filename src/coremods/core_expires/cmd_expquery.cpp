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
#include "brldb/dbflush.h"
#include "brldb/expires.h"
#include "engine.h"
#include "converter.h"
#include "core_expires.h"

CommandTTL::CommandTTL(Module* Creator) : Command(Creator, "TTL", 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandTTL::Handle(User* user, const Params& parameters)
{       
         const std::string& key = parameters[0];
         
         signed int ttl = ExpireManager::TriggerTIME(key, user->select);
         
         if (ttl != -1)
         {
                  user->SendProtocol(BRLD_TTL, key, Daemon::Format("%s expires in %d seconds.", key.c_str(), ((int)ttl - (int)Kernel->Now())).c_str());
         }
         else
         {	
                  /* Unable to remove this 'key' from ExpireManager. */
                  
                  user->SendProtocol(ERR_NOT_EXPIRE, key, Daemon::Format("%s is not expiring", key.c_str()).c_str());
         }
         
         return SUCCESS;
}

CommandPersist::CommandPersist(Module* Creator) : Command(Creator, "PERSIST", 1, 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandPersist::Handle(User* user, const Params& parameters) 
{       
         const std::string& key = parameters[0];
          
         signed int ttl = ExpireManager::TriggerTIME(key, user->select);
         
         if (ttl != -1)
         {
                  if (ExpireManager::Delete(key, user->select))
                  {
                           user->SendProtocol(BRLD_PERSIST, 1, key, Daemon::Format("%s has persisted.", key.c_str()).c_str());
                  }
                  else
                  {
                           user->SendProtocol(ERR_PERSIST, 0, key, Daemon::Format("%s is unable to persist.", key.c_str()).c_str());
                  }
         }
         else
         {
                  user->SendProtocol(ERR_NOT_EXPIRE, key, Daemon::Format("%s is not expiring.", key.c_str()).c_str());
         }
         
         return SUCCESS;

}