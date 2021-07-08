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
#include "engine.h"
#include "converter.h"
#include "core_expires.h"

CommandExpireLIST::CommandExpireLIST(Module* Creator) : Command(Creator, "EXPLIST", 0, 1)
{
         last_empty_ok = true;
         syntax = "<*argument>";
}

COMMAND_RESULT CommandExpireLIST::Handle(User* user, const Params& parameters)
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
                  user->SendProtocol(ERR_INVALID_PARAM, INVALID_TYPE);
                  return FAILED;
         }
         
         /* User has requested all expires with expiration date. */
         
         ExpireMap& expiring = Kernel->Store->Expires->GetExpires();
         
         Dispatcher::JustAPI(user, BRLD_EXPIRE_BEGIN);

         for (ExpireMap::iterator it = expiring.begin(); it != expiring.end(); ++it)
         {
               ExpireEntry entry = it->second;
               
               if (entry.database != user->current_db)
               {
                     continue;
               }
               
               if (entry.select != user->select)
               {
                    continue;
               }
               
               std::string schedule;
               
               if (parameters.size() && arg == "h")
               {
                      schedule = convto_string(entry.schedule);
               }
               else 
               {
                      schedule = Daemon::HumanEpochTime(entry.schedule).c_str();
               }
               
               user->SendProtocol(BRLD_EXPIRE_ITEM, Daemon::Format("%-29s | %5s ", entry.key.c_str(), schedule.c_str()));
         }
         
         Dispatcher::JustAPI(user, BRLD_EXPIRE_END);
         return SUCCESS;
}

CommandReset::CommandReset(Module* Creator) : Command(Creator, "RESET", 0)
{
         
}

COMMAND_RESULT CommandReset::Handle(User* user, const Params& parameters)
{       
         unsigned int counter = Kernel->Store->Expires->CountAll();

         /* Clears all expires pending. */
         
         ExpireManager::Reset();
         user->SendProtocol(BRLD_EXP_DELETED, counter, PROCESS_OK);
         sfalert(user, NOTIFY_DEFAULT, "All expires have been removed: %u", counter);
         return SUCCESS;
}

CommandSelectReset::CommandSelectReset(Module* Creator) : Command(Creator, "SRESET", 0, 1)
{
        syntax = "<select>";
}

COMMAND_RESULT CommandSelectReset::Handle(User* user, const Params& parameters)
{
        std::string use;
       
        if (parameters.size())
        { 
            use = parameters[0];
        
            if (!is_number(use))
            {
                 user->SendProtocol(ERR_USE, DBL_NOT_NUM, MUST_BE_NUMERIC.c_str());
                 return FAILED;
            }

            if (!is_positive_number(use))
            {
                  user->SendProtocol(ERR_USE, ERR_MUST_BE_POS_INT, MUST_BE_POSIT.c_str());
                  return FAILED;
            }
            
            if (!Daemon::CheckRange(user, use, "Must be a value between 1 and 100", 1, 100))
            {
                  return FAILED;
            }
        }
        else
        {
             use = user->select;
        }

        /* Clears all expires pending. */

        unsigned int counter = ExpireManager::SelectReset(user->GetDatabase()->GetName(), use);
        user->SendProtocol(BRLD_INFO_EXP_DEL, PROCESS_OK);
        
        sfalert(user, NOTIFY_DEFAULT, "Expires from select %s have been removed: %u", use.c_str(), counter);
        return SUCCESS;
}


