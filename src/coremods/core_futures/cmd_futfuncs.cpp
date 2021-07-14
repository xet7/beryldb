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
#include "brldb/futures.h"
#include "engine.h"
#include "converter.h"
#include "core_futures.h"

CommandFutureList::CommandFutureList(Module* Creator) : Command(Creator, "FTLIST", 0, 1)
{
         last_empty_ok = true;
         syntax = "<*argument>";
}

COMMAND_RESULT CommandFutureList::Handle(User* user, const Params& parameters)
{
         const std::string& arg = parameters[0];
         
         /* 
          * These two arguments are valid.  Arguments valid are:
          *
          * h: Human readable format.
          * r: Raw format date.
          */
         
         if (parameters.size() && arg != "h" && arg != "r")
         {
                  user->SendProtocol(ERR_INPUT2, ERR_INVALID_PARAM, INVALID_TYPE);
                  return FAILED;
         }
         
         /* User has requested all expires with expiration date. */
         
         FutureMap& futures = Kernel->Store->Futures->GetFutures();
         
         Dispatcher::JustAPI(user, BRLD_EXPIRE_BEGIN);

         for (FutureMap::iterator it = futures.begin(); it != futures.end(); ++it)
         {
               FutureEntry entry = it->second;
               
               std::string schedule;
               
               if (parameters.size() && arg == "h")
               {
                      schedule = convto_string(entry.schedule);
               }
               else 
               {
                      schedule = Daemon::HumanEpochTime(entry.schedule).c_str();
               }
               
               user->SendProtocol(BRLD_FUTURE_ITEM, Daemon::Format("%-29s | %5s | %3s | %5s ", entry.key.c_str(), schedule.c_str(), entry.select.c_str(), entry.database->GetName().c_str()));
         }
         
         Dispatcher::JustAPI(user, BRLD_FUTURE_END);
         return SUCCESS;

}

CommandSelectCount::CommandSelectCount(Module* Creator) : Command(Creator, "FTSELECT", 0, 1)
{
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
         
         FutureMap& expiring = Kernel->Store->Futures->GetFutures();

         Dispatcher::JustAPI(user, BRLD_FUTURE_BEGIN);

         for (FutureMap::iterator it = expiring.begin(); it != expiring.end(); ++it)
         {
               FutureEntry entry = it->second;

               if (entry.select != select || entry.database != user->GetDatabase())
               {  
                         continue;
               }
               
               std::string schedule = Daemon::HumanEpochTime(entry.schedule).c_str();
               user->SendProtocol(BRLD_FUTURE_ITEM, Daemon::Format("%-29s | %5s ", entry.key.c_str(), schedule.c_str()));
         }
         
         Dispatcher::JustAPI(user, BRLD_FUTURE_END);
         return SUCCESS;
}

