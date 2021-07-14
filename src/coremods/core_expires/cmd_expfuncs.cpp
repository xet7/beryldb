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
#include "maker.h"
#include "core_expires.h"

CommandExpireFIND::CommandExpireFIND(Module* Creator) : Command(Creator, "EXPFIND", 1, 2)
{
         last_empty_ok = true;
         requires = 'e';
         syntax = "<key> <argument>";
}

COMMAND_RESULT CommandExpireFIND::Handle(User* user, const Params& parameters)
{
         const std::string& key = parameters[0];
         const std::string& arg = parameters[1];
         
         if (parameters.size() == 2 && arg != "h" && arg != "r")
         {
                  user->SendProtocol(ERR_INPUT2, ERR_INVALID_PARAM, INVALID_TYPE);
                  return FAILED;
         }
         
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
               
               if (!Daemon::Match(entry.key, key))
               {
                      continue;
               }
               
               std::string schedule;

               if (parameters.size() == 2 && arg == "h")
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

CommandExpireLIST::CommandExpireLIST(Module* Creator) : Command(Creator, "EXPLIST", 0, 1)
{
         last_empty_ok = true;
         requires = 'e';
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
                  user->SendProtocol(ERR_INPUT2, ERR_INVALID_PARAM, INVALID_TYPE);
                  return FAILED;
         }
         
         /* User has requested all expires with expiration date. */
         
         ExpireMap& expiring = Kernel->Store->Expires->GetExpires();
         
         Dispatcher::JustAPI(user, BRLD_EXPIRE_BEGIN);

         for (ExpireMap::iterator it = expiring.begin(); it != expiring.end(); ++it)
         {
               ExpireEntry entry = it->second;
               
               std::string schedule;
               
               if (parameters.size() && arg == "h")
               {
                      schedule = convto_string(entry.schedule);
               }
               else 
               {
                      schedule = Daemon::HumanEpochTime(entry.schedule).c_str();
               }
               
               user->SendProtocol(BRLD_EXPIRE_ITEM, Daemon::Format("%-29s | %5s | %3s | %5s ", entry.key.c_str(), schedule.c_str(), entry.select.c_str(), entry.database->GetName().c_str()));
         }
         
         Dispatcher::JustAPI(user, BRLD_EXPIRE_END);
         return SUCCESS;
}

CommandReset::CommandReset(Module* Creator) : Command(Creator, "RESET", 0)
{
         requires = 'e';
}

COMMAND_RESULT CommandReset::Handle(User* user, const Params& parameters)
{       
         unsigned int counter = Kernel->Store->Expires->CountAll();

         /* Clears all expires pending. */
         
         ExpireManager::Reset();
         user->SendProtocol(BRLD_INPUT, PROCESS_OK);
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
        
            if (!CheckValidPos(user, use))
            {
                  return FAILED;
            }
            
            if (!Daemon::CheckRange(user, use, INVALID_RANGE, 1, 100))
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
        user->SendProtocol(BRLD_INPUT, PROCESS_OK);
        
        sfalert(user, NOTIFY_DEFAULT, "Expires from select %s have been removed: %u", use.c_str(), counter);
        return SUCCESS;
}


