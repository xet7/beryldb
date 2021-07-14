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
#include "managers/vectors.h"
#include "engine.h"
#include "maker.h"
#include "core_vector.h"

CommandVPush::CommandVPush(Module* Creator) : Command(Creator, "VPUSH", 2, 2)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVPush::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];
        const std::string value = parameters.back();
        
        if (!CheckFormat(user, value))
        {
            return FAILED;
        }

        VectorHelper::Push(user, key, value);
        return SUCCESS;  
}

CommandVPopFront::CommandVPopFront(Module* Creator) : Command(Creator, "VPOPFRONT", 1, 1)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVPopFront::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];

        VectorHelper::PopFront(user, key);
        return SUCCESS;  
}

CommandVPopBack::CommandVPopBack(Module* Creator) : Command(Creator, "VPOPBACK", 1, 1)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVPopBack::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];

        VectorHelper::PopBack(user, key);
        return SUCCESS;  
}

