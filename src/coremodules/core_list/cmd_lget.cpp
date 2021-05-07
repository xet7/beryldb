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
#include "managers/lists.h"
#include "engine.h"
#include "core_list.h"

CommandLGet::CommandLGet(Module* Creator) : Command(Creator, "LGET", 1, 3)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandLGet::Handle(User* user, const Params& parameters)
{  
       const std::string key = parameters[0];
        
       signed int offset;
       signed int limit;
       
       if (parameters.size() == 2)
       {
             limit = convto_num<signed int>(parameters[1]); 
             offset = 0;
       }
       else if (parameters.size() == 3)
       {
             limit = convto_num<signed int>(parameters[2]); 
             offset = convto_num<signed int>(parameters[1]);
       }
       else
       {
            limit = -1;
            offset = 0;
       }
        

        ListHelper::Get(user, Kernel->Store->Default, user->select, key, offset, limit);
        return SUCCESS;  
}