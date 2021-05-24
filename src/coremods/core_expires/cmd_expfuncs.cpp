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
         
         /* No argument provided, we simply count expire items. */
         
         if (!parameters.size() || arg.length() > 1)
         {
                  unsigned int count = Kernel->Store->Expires->Count(user->current_db, user->select);
                  user->SendProtocol(BRLD_EXP_COUNT, convto_string(count), count);
                  return SUCCESS;
         }
         
         /* 
          * These two arguments are valid.  Arguments valid are:
          *
          * h: Human readable format.
          * r: Raw format date.
          */
         
         if (arg != "h" && arg != "r")
         {
                  user->SendProtocol(ERR_INVALID_PARAM, arg, "Invalid parameter.");
                  return FAILED;
         }
         
         /* User has requested all expires with expiration date. */
         
         ExpireMap& expiring = Kernel->Store->Expires->GetExpires();
         
         /* Counts expires. */
         
         unsigned int counter = 0;

         user->SendProtocol(BRLD_EXPIRE_BEGIN, "Begin of EXPIRE list.");

         for (ExpireMap::iterator it = expiring.begin(); it != expiring.end(); ++it)
         {
               ExpireEntry entry = it->second;
               
               std::string schedule;
               
               if (arg == "h")
               {
                    schedule = Daemon::HumanEpochTime(entry.schedule).c_str();
               }
               else 
               {
                    schedule = convto_string(entry.schedule);
               }
               
               user->SendProtocol(BRLD_EXPIRE_ITEM, entry.key, Daemon::Format("%s | %s", entry.key.c_str(), schedule.c_str()));
         }
         
        user->SendProtocol(BRLD_EXPIRE_END, Daemon::Format("End of EXPIRE list (%u)", counter).c_str());
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
         
         return SUCCESS;
}

CommandSReset::CommandSReset(Module* Creator) : Command(Creator, "SRESET", 0, 1)
{
        syntax = "<select>";
}

COMMAND_RESULT CommandSReset::Handle(User* user, const Params& parameters)
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

        unsigned int counter = ExpireManager::SReset(user->current_db, use);
        user->SendProtocol(BRLD_INFO_EXP_DEL, Daemon::Format("Deleting: %d", counter).c_str());
        user->SendProtocol(BRLD_INFO_EXP_DEL, PROCESS_OK);

        return SUCCESS;
}


