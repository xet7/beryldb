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
#include "managers/maps.h"
#include "engine.h"
#include "core_maps.h"

CommandHGET::CommandHGET(Module* Creator) : Command(Creator, "HGET", 2, 2)
{
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHGET::Handle(User* user, const Params& parameters)
{  
       const std::string kmap = parameters[0];
       const std::string key = parameters[1];

       MapsHelper::Get(user, user->current_db, user->select, kmap, key);
       return SUCCESS;
}


CommandHCount::CommandHCount(Module* Creator) : Command(Creator, "HCOUNT", 1)
{
         syntax = "<map>";
}

COMMAND_RESULT CommandHCount::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap = parameters[0];

       MapsHelper::Count(user, user->current_db, user->select, kmap);
       return SUCCESS;
}


CommandHExists::CommandHExists(Module* Creator) : Command(Creator, "HEXISTS", 2, 2)
{
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHExists::Handle(User* user, const Params& parameters)
{  
       const std::string kmap = parameters[0];
       const std::string key = parameters[1];

       MapsHelper::Get(user, user->current_db, user->select, kmap, key, TYPE_EXIST);
       return SUCCESS;
}

