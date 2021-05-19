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
#include "core_futures.h"

CommandFuture::CommandFuture(Module* Creator) : Command(Creator, "FUTURE", 3, 3)
{
          syntax = "<seconds> <key> <value>";
}

COMMAND_RESULT CommandFuture::Handle(User* user, const Params& parameters) 
{       
          const std::string& seconds = parameters[0];
          const std::string& key = parameters[1];
          const std::string& value = parameters[2];
          
          if (!is_number(seconds))
          {
                 user->SendProtocol(ERR_FUTURE, seconds, MUST_BE_NUMERIC.c_str());
                 return FAILED;
          }

          if (!is_positive_number(seconds))
          {
                 user->SendProtocol(ERR_FUTURE, seconds, MUST_BE_POSIT.c_str());
                 return FAILED;
          }
  
          if (!Daemon::CheckFormat(user, value))
          {
               return FAILED;
          }   

          unsigned int exp_usig = convto_num<unsigned int>(seconds);
          Kernel->Store->Futures->Add(user->current_db, exp_usig, key, value, user->select, false);
          user->SendProtocol(BRLD_FUTURE_ADD, key, value, PROCESS_OK);
          return SUCCESS;
}

CommandTTE::CommandTTE(Module* Creator) : Command(Creator, "TTE", 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandTTE::Handle(User* user, const Params& parameters)
{
         const std::string& key = parameters[0];
         
         signed int ttl = FutureManager::TriggerTIME(user->current_db, key, user->select);
         
         if (ttl != -1)
         {
                  user->SendProtocol(BRLD_TTE, key, user->select, ((int)ttl - (int)Kernel->Now()));
         }
         else
         {      
                  /* Unable to remove this 'key' from ExpireManager. */
                  
                  user->SendProtocol(ERR_NOT_FUTURE, key, user->select, "0");
         }
         
         return SUCCESS;
}

CommandExec::CommandExec(Module* Creator) : Command(Creator, "EXEC", 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandExec::Handle(User* user, const Params& parameters)
{
         const std::string& key = parameters[0];
         
         bool result = FutureManager::Exec(user->current_db, key, user->select);
         
         if (result)
         {
               user->SendProtocol(BRLD_FUTURE_EXEC, key, user->select, PROCESS_OK); 
         }
         else
         {      
               user->SendProtocol(ERR_NOT_FUTURE, key, user->select, Daemon::Format("Future not found: %s", key.c_str()).c_str());
               return FAILED;
         }

         return SUCCESS;
}

CommandFResetAll::CommandFResetAll(Module* Creator) : Command(Creator, "FRESETALL", 0, 0)
{

}

COMMAND_RESULT CommandFResetAll::Handle(User* user, const Params& parameters)
{
         user->SendProtocol(BRLD_INFO_FUT_DEL, Kernel->Store->Futures->CountAll(),  Kernel->Store->Futures->CountAll());

         FutureManager::Reset();
         user->SendProtocol(BRLD_FUTURE_DELETED, "Futures removed.");
         return SUCCESS;
}

CommandFReset::CommandFReset(Module* Creator) : Command(Creator, "FRESET", 0, 1)
{
        syntax = "<select>";
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

        unsigned int counter = FutureManager::SReset(user->current_db, use);
        user->SendProtocol(BRLD_INFO_EXP_DEL, counter, counter);

        return SUCCESS;
}

CommandCancel::CommandCancel(Module* Creator) : Command(Creator, "CANCEL", 1, 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandCancel::Handle(User* user, const Params& parameters) 
{
        const std::string& key = parameters[0];
        
        if (FutureManager::Delete(user->current_db, key, user->select))
        {
              user->SendProtocol(BRLD_FUTURE_DEL, key, user->select, PROCESS_OK);
        }
        else
        {
              user->SendProtocol(ERR_FUTURE_NOT_FOUND, key, user->select, Daemon::Format("Future not found: %s", key.c_str()).c_str());
              return FAILED;
        }
        
        return SUCCESS;
}

CommandFKey::CommandFKey(Module* Creator) : Command(Creator, "FKEY", 1, 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandFKey::Handle(User* user, const Params& parameters) 
{
        const std::string& key = parameters[0];

        std::tuple<int, std::string> response = FutureManager::GetVal(user->current_db, key, user->select);
        
        if (std::get<0>(response))
        {
               FutureManager::Delete(user->current_db, key, user->select);
               user->SendProtocol(BRLD_FUTURE_DEL, key, user->select, PROCESS_OK, PROCESS_OK);
        }
        else
        {
               user->SendProtocol(ERR_FUTURE_NOT_FOUND, key, user->select, 0, "0");
               return FAILED;
        }
        
        return SUCCESS;
}

CommandFValue::CommandFValue(Module* Creator) : Command(Creator, "FVALUE", 1, 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandFValue::Handle(User* user, const Params& parameters) 
{
        const std::string& key = parameters[0];
        
        std::tuple<int, std::string> response = FutureManager::GetVal(user->current_db, key, user->select);
        
        if (std::get<0>(response))
        {
               user->SendProtocol(BRLD_FUTURE_VALUE, key, user->select, std::get<1>(response));
        }
        else
        {
               user->SendProtocol(ERR_FUTURE_NOT_FOUND, key, user->select, 0, "0");
               return FAILED;
        }
        
        return SUCCESS;
}        


