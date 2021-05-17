/*
 * BerylDB - A modular database.
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
#include "managers/keys.h"
#include "engine.h"
#include "core_operations.h"

CommandIncr::CommandIncr(Module* Creator) : Command(Creator, "INCR", 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandIncr::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];

        KeyHelper::Operation(user, Kernel->Store->Default, user->select, key, OP_INCR);
        return SUCCESS;  
}

CommandIncrBy::CommandIncrBy(Module* Creator) : Command(Creator, "INCRBY", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandIncrBy::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters[1];
        
        if (!is_number(value, true))
        {
                user->SendProtocol(ERR_MUST_BE_DOUBLE, Daemon::Format("%s", MUST_BE_NUMERIC.c_str()));
                return FAILED;
        }

        KeyHelper::Operation(user, Kernel->Store->Default, user->select, key, OP_ADD, value);
        return SUCCESS;  
}