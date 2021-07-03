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

CommandHDel::CommandHDel(Module* Creator) : Command(Creator, "HDEL", 2, 2)
{
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHDel::Handle(User* user, const Params& parameters)
{  
       const std::string kmap = parameters[0];
       const std::string key = parameters[1];

       MapsHelper::Delete(user, kmap, key);
       return SUCCESS;
}

CommandHDelAll::CommandHDelAll(Module* Creator) : Command(Creator, "HDELALL", 1, 1)
{
         syntax = "<map>";
}

COMMAND_RESULT CommandHDelAll::Handle(User* user, const Params& parameters)
{  
       const std::string kmap = parameters[0];

       //MapsHelper::DeleteAll(user, user->current_db, user->select, kmap);
       return SUCCESS;
}