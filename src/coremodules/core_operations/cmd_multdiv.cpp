/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
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
#include "managers/keys.h"
#include "engine.h"
#include "core_operations.h"

CommandDiv::CommandDiv(Module* Creator) : Command(Creator, "DIV", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandDiv::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters[1];
        
        if (!is_number(value, true))
        {
                user->SendProtocol(ERR_MUST_BE_DOUBLE, Daemon::Format("%s", MUST_REAL_MSG.c_str()));
                return FAILED;
        }

        KeyHelper::Operation(user, Kernel->Store->Default, user->select, key, OP_DIV, value);
        return SUCCESS;  
}

CommandMult::CommandMult(Module* Creator) : Command(Creator, "MULT", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandMult::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters[1];
        
        if (!is_number(value, true))
        {
                user->SendProtocol(ERR_MUST_BE_DOUBLE, Daemon::Format("%s", MUST_REAL_MSG.c_str()));
                return FAILED;
        }

        KeyHelper::Operation(user, Kernel->Store->Default, user->select, key, OP_MULT, value);
        return SUCCESS;  
}