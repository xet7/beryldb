/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
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

CommandHSET::CommandHSET(Module* Creator) : Command(Creator, "HSET", 3, 3)
{
         syntax = "<map> <key> <value>";
}

COMMAND_RESULT CommandHSET::Handle(User* user, const Params& parameters)
{  
       const std::string kmap  = parameters[0];
       const std::string key   = parameters[1];
       const std::string value = parameters.back();

       if (!Daemon::CheckFormat(user, value))
       {
            return FAILED;
       }
       
       MapsHelper::Set(user, user->current_db, user->select, kmap, key, value);
       return SUCCESS;
}

CommandHMove::CommandHMove(Module* Creator) : Command(Creator, "HMOVE", 3, 3)
{
         syntax = "<map> <hash> <dest>";
}

COMMAND_RESULT CommandHMove::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap  = parameters[0];
       const std::string& key   = parameters[1];
       const std::string& dest   = parameters[2];

       MapsHelper::Move(user, user->current_db, user->select, kmap, key, dest);
       return SUCCESS;
}
