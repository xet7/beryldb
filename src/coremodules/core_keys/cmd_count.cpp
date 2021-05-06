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

CommandCount::CommandCount(Module* Creator) : Command(Creator, "COUNT", 0, 1)
{
         syntax = "<\%key>";
}

COMMAND_RESULT CommandCount::Handle(User* user, const Params& parameters)
{  
       std::string key;

       /* We use a wildcardcard if no key is provided. */
       
       if (!parameters.size())
       {
            key = "*";
       }
       else
       {
             key = parameters[0];
       }
       
       KeyHelper::Count(user, user->current_db, user->select, key);
       return SUCCESS;
}

CommandTouch::CommandTouch(Module* Creator) : Command(Creator, "TOUCH", 1, 1)
{
         syntax = "[keys]";
}

COMMAND_RESULT CommandTouch::Handle(User* user, const Params& parameters)
{  
       const std::string& keys = parameters.back();

       KeyHelper::Touch(user, user->current_db, user->select, keys);
       return SUCCESS;
}

CommandNTouch::CommandNTouch(Module* Creator) : Command(Creator, "NTOUCH", 1, 1)
{
         syntax = "[keys]";
}

COMMAND_RESULT CommandNTouch::Handle(User* user, const Params& parameters)
{  
       const std::string& keys = parameters.back();

       KeyHelper::Touch(user, user->current_db, user->select, keys, "", TYPE_NTOUCH);
       return SUCCESS;
}

CommandTCount::CommandTCount(Module* Creator) : Command(Creator, "TCOUNT", 1, 1)
{
         syntax = "[keys]";
}

COMMAND_RESULT CommandTCount::Handle(User* user, const Params& parameters)
{  
       const std::string& keys = parameters.back();

       KeyHelper::Touch(user, user->current_db, user->select, keys, "", TYPE_TCOUNT);
       return SUCCESS;
}

CommandConcat::CommandConcat(Module* Creator) : Command(Creator, "CONCAT", 1, 1)
{
         syntax = "[keys]";
}

COMMAND_RESULT CommandConcat::Handle(User* user, const Params& parameters)
{  
       const std::string& keys = parameters.back();

       KeyHelper::Touch(user, user->current_db, user->select, keys, "", TYPE_CONCAT);
       return SUCCESS;
}

CommandSConcat::CommandSConcat(Module* Creator) : Command(Creator, "SCONCAT", 2, 2)
{
         syntax = "<div> [keys]";
}

COMMAND_RESULT CommandSConcat::Handle(User* user, const Params& parameters)
{  
       const std::string& div = parameters[0];
       const std::string& keys = parameters.back();

       KeyHelper::Touch(user, user->current_db, user->select, keys, div, TYPE_SCONCAT);
       return SUCCESS;
}
