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

CommandLPush::CommandLPush(Module* Creator) : Command(Creator, "LPUSH", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandLPush::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];
        const std::string value = parameters.back();
        
        if (!Daemon::CheckFormat(user, value))
        {
            return FAILED;
        }

        ListHelper::Push(user, key, value);
        return SUCCESS;  
}

CommandLMove::CommandLMove(Module* Creator) : Command(Creator, "LMOVE", 3, 3)
{
         syntax = "<key> <select> <value>";
}

COMMAND_RESULT CommandLMove::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];
        const std::string select = parameters[1];
        const std::string back = parameters.back();

        if (!Daemon::CheckFormat(user, back))
        {
            return FAILED;
        }
        
        //ListHelper::Move(user, Kernel->Store->GetDefault(), user->select, select, key, back);
        return SUCCESS;  
}

