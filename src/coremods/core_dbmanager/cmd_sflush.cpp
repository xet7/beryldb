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
#include "engine.h"

#include "brldb/dbmanager.h"
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "brldb/database.h"
#include "maker.h"
#include "managers/databases.h"
#include "converter.h"
#include "engine.h"
#include "core_dbmanager.h"

CommandSFlush::CommandSFlush(Module* Creator) : Command(Creator, "SFLUSH", 0, 1)
{
         requires = 'r';
         syntax = "<select>";
}

COMMAND_RESULT CommandSFlush::Handle(User* user, const Params& parameters)
{  
       std::string select;
       
       if (parameters.size() > 0)
       {
             select = parameters[0];
       }
       else
       {
            select = user->select;
       }
       
       if (CheckValidPos(user, select))
       {
              return FAILED;
       }
       
       if (!Daemon::CheckRange(user, select, INVALID_RANGE, 1, 100))
       {
              return FAILED;
       }

       DBHelper::SFlush(user, select);
       return SUCCESS;
}

CommandUsing::CommandUsing(Module* Creator) : Command(Creator, "USING", 1, 1)
{
         requires = 'm';
         syntax = "<instance>";
}

COMMAND_RESULT CommandUsing::Handle(User* user, const Params& parameters)
{  
       const std::string& instance = parameters[0];

       User* found = Kernel->Clients->FindInstance(instance);

       if (!found)
       {
              user->SendProtocol(ERR_INPUT, NOT_FOUND);
              return FAILED;
       }

       user->SendProtocol(BRLD_OK, found->select.c_str());
       return SUCCESS;
}

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

       if (!CheckValidPos(user, use))
       {
              return FAILED;
       }

       if (!Daemon::CheckRange(user, use, INVALID_RANGE, 1, 100))
       {
               return FAILED;
       }

       if (user->select == use)
       {
             user->SendProtocol(ERR_INPUT, PROCESS_ALREADY);
             return FAILED;
       }

       user->select = use;
       user->SendProtocol(BRLD_NEW_USE, use, PROCESS_OK);

       return SUCCESS;
}

CommandCurrent::CommandCurrent(Module* Creator) : Command(Creator, "CURRENT", 0)
{

}

COMMAND_RESULT CommandCurrent::Handle(User* user, const Params& parameters)
{  
       const std::string use = user->select;
       user->SendProtocol(BRLD_OK, use.c_str());
       return SUCCESS;
}

CommandDBReset::CommandDBReset(Module* Creator) : Command(Creator, "DBRESET", 0)
{
       requires = 'e';
}			

COMMAND_RESULT CommandDBReset::Handle(User* user, const Params& parameters)
{  
       if (!Kernel->Store->Flusher->Status())
       {
              user->SendProtocol(ERR_INPUT, ALREADY_PAUSED);
              return FAILED;
       }
       
       DataFlush::ResetAll();
       user->SendProtocol(BRLD_OK, PROCESS_OK);
       return SUCCESS;
}

CommandDBSize::CommandDBSize(Module* Creator) : Command(Creator, "DBSIZE", 0)
{
       group = 'w';
}

COMMAND_RESULT CommandDBSize::Handle(User* user, const Params& parameters)
{  
       DBHelper::DBSize(user);
       return SUCCESS;
}



