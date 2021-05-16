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
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "managers/databases.h"
#include "converter.h"
#include "engine.h"
#include "core_dbmanager.h"

CommandUse::CommandUse(Module* Creator) : Command(Creator, "USE", 1)
{          
         no_hint_until_reg = true;
         
         /* Users may provide use before registering. */
         
         pre_reg_ok = true;
         syntax = "<id between 1 and 100>";
}

COMMAND_RESULT CommandUse::Handle(User* user, const Params& parameters)
{  
       const std::string& use = parameters[0];

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
       
       if (user->select == use)
       {
             user->SendProtocol(ERR_USE, use, Daemon::Format("You are already using %s", use.c_str()).c_str());
             return FAILED;
       }
       
      
       user->select = use;
       user->SendProtocol(BRLD_NEW_USE, use, Daemon::Format("Selected changed to %s", use.c_str()).c_str());
       
       return SUCCESS;
}

CommandCurrent::CommandCurrent(Module* Creator) : Command(Creator, "CURRENT", 0)
{

}

COMMAND_RESULT CommandCurrent::Handle(User* user, const Params& parameters)
{  
       const std::string use = user->select;
       user->SendProtocol(BRLD_CURRENT_USE, use, Daemon::Format("You are using %s", use.c_str()).c_str());
       return SUCCESS;
}

CommandDBReset::CommandDBReset(Module* Creator) : Command(Creator, "DBRESET", 0)
{

}

COMMAND_RESULT CommandDBReset::Handle(User* user, const Params& parameters)
{  
       if (!Kernel->Store->Flusher->Status())
       {
              user->SendProtocol(ERR_UNABLE_DBRESET, "Already paused.");
              return FAILED;
       }
       
       DataFlush::ResetAll();
       
       user->SendProtocol(BRLD_DB_RESET, PROCESS_OK);
       
       return SUCCESS;
}

CommandDBSize::CommandDBSize(Module* Creator) : Command(Creator, "DBSIZE", 0)
{

}

COMMAND_RESULT CommandDBSize::Handle(User* user, const Params& parameters)
{  
       DBHelper::DBSize(user, user->current_db);
       return SUCCESS;
}