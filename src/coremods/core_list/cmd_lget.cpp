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

CommandLSearch::CommandLSearch(Module* Creator) : Command(Creator, "LSEARCH", 1, 3)
{
         syntax = "<%key> <offset> <limit>";
}

COMMAND_RESULT CommandLSearch::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
        
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


        ListHelper::Search(user, Kernel->Store->Default, user->select, key, offset, limit);
        return SUCCESS;  
}

CommandLFind::CommandLFind(Module* Creator) : Command(Creator, "LFIND", 2, 4)
{
         syntax = "<key> <%value> <offset> <limit>";
}

COMMAND_RESULT CommandLFind::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];

       signed int offset;
       signed int limit;

       if (parameters.size() == 3)
       {
             if (!is_zero_or_great(parameters[2]))
             {
                 user->SendProtocol(ERR_USE, ERR_GREAT_ZERO, MUST_BE_GREAT_ZERO.c_str());
                return FAILED;
             }
       
             limit = convto_num<signed int>(parameters[2]); 
             offset = 0;
       }
       else if (parameters.size() == 4)
       {
             if (!is_zero_or_great(parameters[3]) || !is_zero_or_great(parameters[2]))
             {
                   user->SendProtocol(ERR_USE, ERR_GREAT_ZERO, MUST_BE_GREAT_ZERO.c_str());
                  return FAILED;
             }
       
             limit = convto_num<signed int>(parameters[3]); 
             offset = convto_num<signed int>(parameters[2]);
       }
       else
       {
            limit = -1;
            offset = 0;
       }
       
       if (!Daemon::CheckFormat(user, value))
       {
            return FAILED;
       }

       ListHelper::Find(user, Kernel->Store->Default, user->select, key, value, offset, limit);
       return SUCCESS;  
}
