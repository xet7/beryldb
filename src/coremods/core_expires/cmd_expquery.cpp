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
#include "managers/expires.h"
#include "brldb/expires.h"
#include "engine.h"
#include "converter.h"
#include "core_expires.h"

CommandTTL::CommandTTL(Module* Creator) : Command(Creator, "TTL", 1)
{
         group = 'e';
         syntax = "<key>";
}

COMMAND_RESULT CommandTTL::Handle(User* user, const Params& parameters)
{       
         const std::string& key = parameters[0];
         
         signed int ttl = ExpireManager::GetTIME(user->GetDatabase(), key, user->select);
         
         if (ttl != -1)
         {
                  user->SendProtocol(BRLD_OK2, static_cast<unsigned int>((int)ttl - (int)Kernel->Now()));
         }
         else
         {	
                  /* Unable to remove this 'key' from ExpireManager. */
                  
                  user->SendProtocol(ERR_INPUT2, ERR_NOT_EXPIRE, PROCESS_NULL);
         }
         
         return SUCCESS;
}

CommandTTLAT::CommandTTLAT(Module* Creator) : Command(Creator, "TTLAT", 1)
{
         group = 'e';
         syntax = "<key>";
}

COMMAND_RESULT CommandTTLAT::Handle(User* user, const Params& parameters)
{       
         const std::string& key = parameters[0];
         
         signed int ttl = ExpireManager::GetTIME(user->GetDatabase(), key, user->select);
         
         if (ttl != -1)
         {
                  user->SendProtocol(BRLD_OK2, static_cast<unsigned int>((int)ttl));
         }
         else
         {      
                  /* Unable to remove this 'key' from ExpireManager. */
                  
                  user->SendProtocol(ERR_INPUT2, ERR_NOT_EXPIRE, PROCESS_NULL);
         }
         
         return SUCCESS;
}

CommandPersist::CommandPersist(Module* Creator) : Command(Creator, "PERSIST", 1, 1)
{
         group = 'e';
         syntax = "<key>";
}

COMMAND_RESULT CommandPersist::Handle(User* user, const Params& parameters) 
{       
         const std::string& key = parameters[0];
          
         signed int ttl = ExpireManager::GetTIME(user->GetDatabase(), key, user->select);
         
         if (ttl != -1)
         {
                 ExpireHelper::QuickPersist(user, key);
         }
         else
         {
                 user->SendProtocol(ERR_INPUT2, ERR_PERSIST, PROCESS_NULL);
         }
         
         return SUCCESS;

}

CommandSelectCount::CommandSelectCount(Module* Creator) : Command(Creator, "EXPSELECT", 0, 1)
{
         group = 'e';
         syntax = "<select>";
}

COMMAND_RESULT CommandSelectCount::Handle(User* user, const Params& parameters) 
{
         std::string select;
         
         /* No argument provided, we simply count expire items. */
         
         if (!parameters.size())
         {
                  select = user->select;
         }
         else
         {
                  select  = parameters[0];
         }
         
         ExpireMap& expiring = Kernel->Store->Expires->GetExpires();

         Dispatcher::JustAPI(user, BRLD_EXPIRE_BEGIN);

         for (ExpireMap::iterator it = expiring.begin(); it != expiring.end(); ++it)
         {
               ExpireEntry entry = it->second;

               if (entry.select != select || entry.database != user->GetDatabase())
               {  
                         continue;
               }
               
               std::string schedule = Daemon::HumanEpochTime(entry.schedule).c_str();
               user->SendProtocol(BRLD_EXPIRE_ITEM, Daemon::Format("%-29s | %5s ", entry.key.c_str(), schedule.c_str()));
         }
         
         Dispatcher::JustAPI(user, BRLD_EXPIRE_END);
         return SUCCESS;
}

         
