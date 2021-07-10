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
#include "maker.h"
#include "core_list.h"

CommandLExist::CommandLExist(Module* Creator) : Command(Creator, "LEXISTS", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandLExist::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];

       if (!CheckFormat(user, value))
       {
           return FAILED;
       }
       
       ListHelper::Exist(user, key, value);
       return SUCCESS;  
}

CommandLCount::CommandLCount(Module* Creator) : Command(Creator, "LCOUNT", 1, 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandLCount::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       ListHelper::Count(user, key);
       return SUCCESS;  
}
