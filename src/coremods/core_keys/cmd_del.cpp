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
#include "core_keys.h"

CommandDel::CommandDel(Module* Creator) : Command(Creator, "DEL", 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandDel::Handle(User* user, const Params& parameters)
{  
       const std::string key = parameters[0];
       
       KeyHelper::Delete(user, user->current_db, user->select, key, false);
       return SUCCESS;
}