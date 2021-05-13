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
#include "core_keys.h"

CommandFind::CommandFind(Module* Creator) : Command(Creator, "FIND", 1, 3)
{
         syntax = "<\%key> <offset> <limit>";
}

COMMAND_RESULT CommandFind::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       signed int offset;
       signed int limit;

       if (parameters.size() == 2)
       {
             if (!is_zero_or_great(parameters[1]))
             {
                   user->SendProtocol(ERR_USE, ERR_GREAT_ZERO, MUST_BE_GREAT_ZERO.c_str());
                   return FAILED;
             }

             limit = convto_num<signed int>(parameters[1]); 
             offset = 0;
       }
       else if (parameters.size() == 3)
       {
             limit = convto_num<signed int>(parameters[2]); 
             offset = convto_num<signed int>(parameters[1]);
             
             if (!is_zero_or_great(parameters[1]) || !is_zero_or_great(parameters[2]))
             {
                   user->SendProtocol(ERR_USE, ERR_GREAT_ZERO, MUST_BE_GREAT_ZERO.c_str());
                   return FAILED;
             }
       }
       else
       {
            limit = -1;
            offset = 0;
       }
       
       KeyHelper::Find(user, user->current_db, user->select, key, offset, limit);
       return SUCCESS;
}

CommandSearch::CommandSearch(Module* Creator) : Command(Creator, "SEARCH", 1, 3)
{
         syntax = "<\%key> <offset> <limit>";
}

COMMAND_RESULT CommandSearch::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!Daemon::CheckFormat(user, key))
       {
            return FAILED;
       }
       
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

       KeyHelper::Search(user, user->current_db, user->select, key, offset, limit);
       return SUCCESS;
}

CommandRKey::CommandRKey(Module* Creator) : Command(Creator, "RKEY", 0, 0)
{

}

COMMAND_RESULT CommandRKey::Handle(User* user, const Params& parameters)
{
       KeyHelper::Find(user, user->current_db, user->select, "", 0, 0, TYPE_RAKEY);
       return SUCCESS;
}
