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
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "managers/lists.h"
#include "engine.h"
#include "core_list.h"

CommandPop::CommandPop(Module* Creator) : Command(Creator, "LPOP", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandPop::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];
        const std::string value = parameters.back();
        
        if (!Daemon::CheckFormat(user, value))
        {
            return FAILED;
        }
        
        //ListHelper::Delete(user, Kernel->Store->GetDefault(), user->select, key, value, true);
        return SUCCESS;  
}

CommandPopAll::CommandPopAll(Module* Creator) : Command(Creator, "LPOPALL", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandPopAll::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];
        const std::string value = parameters.back();

        if (!Daemon::CheckFormat(user, value))
        {
           return FAILED;
        }

        //ListHelper::Delete(user, Kernel->Store->GetDefault(), user->select, key, value, false);

        return SUCCESS;  
}