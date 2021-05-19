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

CommandFutureCount::CommandFutureCount(Module* Creator) : Command(Creator, "FUTCOUNT", 0, 1)
{
         last_empty_ok = true;
         syntax = "<*argument>";
}

COMMAND_RESULT CommandFutureCount::Handle(User* user, const Params& parameters)
{
         const std::string& arg = parameters[0];
         std::string opt;
         
         if (!parameters.size() || arg.length() > 1)
         {
                  unsigned int count = Kernel->Store->Futures->Count(user->current_db, user->select);
                  user->SendProtocol(BRLD_EXP_COUNT, convto_string(count), count);
                  return SUCCESS;
         }
         
         if (arg != "h" && arg != "r")
         {
                  user->SendProtocol(ERR_INVALID_PARAM, arg, "Invalid parameter.");
                  return FAILED;
         }
          
         FutureMap& expiring = Kernel->Store->Futures->GetFutures();
         
         unsigned int counter = 0;

         user->SendProtocol(BRLD_FUTURE_BEGIN, "Begin of FUTURE list.");

         for (FutureMap::iterator it = expiring.begin(); it != expiring.end(); ++it)
         {
               FutureEntry entry = it->second;
               
               if (opt.empty() || opt != "a")
               {
                     if (entry.select != user->select)
                     {  
                         continue;
                     }
               }
               
               std::string schedule;
               
               if (arg == "h")
               {
                    schedule = Daemon::HumanEpochTime(entry.schedule).c_str();
               }
               else 
               {
                    schedule = convto_string(entry.schedule);
               }
               
               user->SendProtocol(BRLD_FUTURE_ITEM, entry.key, Daemon::Format("%s | %s", entry.key.c_str(), schedule.c_str()));
         }
         
        user->SendProtocol(BRLD_FUTURE_END, Daemon::Format("End of FUTURE list (%u)", counter).c_str());
        return SUCCESS;
}

