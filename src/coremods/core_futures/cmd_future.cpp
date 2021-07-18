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
#include "maker.h"
#include "brldb/dbmanager.h"
#include "brldb/dbflush.h"
#include "brldb/expires.h"
#include "managers/expires.h"
#include "managers/globals.h"
#include "engine.h"
#include "converter.h"
#include "core_futures.h"

CommandFuture::CommandFuture(Module* Creator) : Command(Creator, "FUTURE", 3, 3)
{
         group = 'f';
         syntax = "<seconds> <key> <value>";
}

COMMAND_RESULT CommandFuture::Handle(User* user, const Params& parameters) 
{       
          const std::string& seconds = parameters[0];
          const std::string& key = parameters[1];
          const std::string& value = parameters.back();
          
          if (!CheckValidPos(user, seconds))
          {
              return FAILED;
          }
  
          if (!CheckFormat(user, value))
          {
               return FAILED;
          }   

          unsigned int exp_usig = convto_num<unsigned int>(seconds);
          ExpireHelper::Future(user, key, exp_usig, value);
          return SUCCESS;
}

CommandTTE::CommandTTE(Module* Creator) : Command(Creator, "TTE", 1)
{
         group = 'f';
         syntax = "<key>";
}

COMMAND_RESULT CommandTTE::Handle(User* user, const Params& parameters)
{
         const std::string& key = parameters[0];
         
         signed int ttl = FutureManager::GetTIME(user->GetDatabase(), key, user->select);
         
         if (ttl != -1)
         {
                  user->SendProtocol(BRLD_OK, static_cast<unsigned int>((int)ttl - (int)Kernel->Now()));
         }
         else
         {      
                  /* Unable to remove this 'key' from ExpireManager. */
                  
                  user->SendProtocol(ERR_INPUT, PROCESS_NULL);
                  return FAILED;
         }
         
         return SUCCESS;
}

CommandExec::CommandExec(Module* Creator) : Command(Creator, "EXEC", 1, 1)
{
         group = 'f';
         syntax = "<key>";
}

COMMAND_RESULT CommandExec::Handle(User* user, const Params& parameters)
{
         const std::string& key = parameters[0];
         GlobalHelper::UserFutureExecute(user, key);
         return SUCCESS;
}

CommandFResetAll::CommandFResetAll(Module* Creator) : Command(Creator, "FRESETALL", 0, 0)
{
       requires = 'e';
}

COMMAND_RESULT CommandFResetAll::Handle(User* user, const Params& parameters)
{
       FutureManager::Reset();
       user->SendProtocol(BRLD_OK, PROCESS_OK);
       return SUCCESS;
}

CommandFReset::CommandFReset(Module* Creator) : Command(Creator, "FRESET", 0, 1)
{
        group = 'f';        
        syntax = "<*select>";
}

COMMAND_RESULT CommandFReset::Handle(User* user, const Params& parameters)
{
        std::string use;

        if (parameters.size())
        { 
            use = parameters[0];

            if (!CheckValid(user, use))
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

        if (user->GetDatabase() && user->GetDatabase()->IsClosing())
        {
              user->SendProtocol(ERR_INPUT, DATABASE_BUSY);
              return FAILED;
        }

        Kernel->Store->Futures->SelectReset(user->GetDatabase()->GetName(), use);
        user->SendProtocol(BRLD_OK, PROCESS_OK);
        return SUCCESS;
}

CommandCancel::CommandCancel(Module* Creator) : Command(Creator, "CANCEL", 1, 1)
{
         group = 'f';
         syntax = "<key>";
}

COMMAND_RESULT CommandCancel::Handle(User* user, const Params& parameters) 
{
        const std::string& key = parameters[0];
        
        GlobalHelper::FutureCancel(user, key);
        return SUCCESS;
}

CommandFutureAT::CommandFutureAT(Module* Creator) : Command(Creator, "FUTUREAT", 3, 3)
{
          group = 'f';
          syntax = "<epoch time> <key> <value>";
}

COMMAND_RESULT CommandFutureAT::Handle(User* user, const Params& parameters) 
{    
          const std::string& seconds = parameters[0];
          const std::string& key = parameters[1];
          const std::string& value = parameters[1];
          
          if (!CheckValid(user, seconds))
          {
              return FAILED;
          }

          if (!CheckFormat(user, value))
          {
               return FAILED;
          }   

          unsigned int exp_usig = convto_num<unsigned int>(seconds);

          if ((time_t)exp_usig < Kernel->Now())
          {
                 user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
                 return FAILED;
          }
          
          ExpireHelper::FutureAT(user, key, exp_usig, value);
          return SUCCESS;
}
