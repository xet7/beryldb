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
#include "managers/globals.h"
#include "engine.h"
#include "converter.h"
#include "core_futures.h"

CommandFuture::CommandFuture(Module* Creator) : Command(Creator, "FUTURE", 3, 3)
{
          syntax = "<seconds> <key> <value>";
}

COMMAND_RESULT CommandFuture::Handle(User* user, const Params& parameters) 
{       
          const std::string& seconds = parameters[0];
          const std::string& key = parameters[1];
          const std::string& value = parameters.back();
          
          if (!is_number(seconds))
          {
                 user->SendProtocol(ERR_FUTURE, MUST_BE_NUMERIC);
                 return FAILED;
          }

          if (!is_positive_number(seconds))
          {
                 user->SendProtocol(ERR_FUTURE, MUST_BE_POSIT);
                 return FAILED;
          }
  
          if (!Daemon::CheckFormat(user, value))
          {
               return FAILED;
          }   

          unsigned int exp_usig = convto_num<unsigned int>(seconds);
          ExpireHelper::Future(user, key, exp_usig, value);
          return SUCCESS;
}

CommandTTE::CommandTTE(Module* Creator) : Command(Creator, "TTE", 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandTTE::Handle(User* user, const Params& parameters)
{
         const std::string& key = parameters[0];
         
         signed int ttl = FutureManager::GetTIME(user->current_db, key, user->select);
         
         if (ttl != -1)
         {
                  user->SendProtocol(BRLD_TTE, static_cast<unsigned int>((int)ttl - (int)Kernel->Now()));
         }
         else
         {      
                  /* Unable to remove this 'key' from ExpireManager. */
                  
                  user->SendProtocol(ERR_NOT_FUTURE, PROCESS_NULL);
                  return FAILED;
         }
         
         return SUCCESS;
}

CommandExec::CommandExec(Module* Creator) : Command(Creator, "EXEC", 1, 1)
{
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

}

COMMAND_RESULT CommandFResetAll::Handle(User* user, const Params& parameters)
{
         FutureManager::Reset();
         user->SendProtocol(BRLD_FUTURE_DELETED, PROCESS_OK);
         return SUCCESS;
}

CommandFReset::CommandFReset(Module* Creator) : Command(Creator, "FRESET", 0, 1)
{
        syntax = "<*select>";
}

COMMAND_RESULT CommandFReset::Handle(User* user, const Params& parameters)
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

        FutureManager::SelectReset(user->current_db->GetName(), use);
        user->SendProtocol(BRLD_INFO_EXP_DEL, PROCESS_OK);
        return SUCCESS;
}

CommandCancel::CommandCancel(Module* Creator) : Command(Creator, "CANCEL", 1, 1)
{
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
          syntax = "<epoch time> <key> <value>";
}

COMMAND_RESULT CommandFutureAT::Handle(User* user, const Params& parameters) 
{    
          const std::string& seconds = parameters[0];
          const std::string& key = parameters[1];
          const std::string& value = parameters[1];
          
          if (!is_number(seconds))
          {
                 user->SendProtocol(ERR_FUTURE, MUST_BE_NUMERIC);
                 return FAILED;
          }

          if (!is_positive_number(seconds))
          {
                 user->SendProtocol(ERR_FUTURE, MUST_BE_POSIT);
                 return FAILED;
          }

          if (!Daemon::CheckFormat(user, value))
          {
               return FAILED;
          }   
          
          unsigned int exp_usig = convto_num<unsigned int>(seconds);
          ExpireHelper::FutureAT(user, key, exp_usig, value);
          return SUCCESS;
}
