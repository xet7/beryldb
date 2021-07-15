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
#include "maker.h"
#include "managers/maps.h"
#include "engine.h"
#include "core_maps.h"

CommandHSet::CommandHSet(Module* Creator) : Command(Creator, "HSET", 3, 3)
{
         group = 'm';
         syntax = "<map> <key> <value>";
}

COMMAND_RESULT CommandHSet::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap  = parameters[0];
       const std::string& key   = parameters[1];
       const std::string& value = parameters.back();

        if (!CheckKey(user, key) || !CheckKey(user, kmap))
        {
            return FAILED;
        }

       if (!CheckFormat(user, value))
       {
            return FAILED;
       }
       
       MapsHelper::Set(user, kmap, key, value);
       return SUCCESS;
}

CommandHSetNX::CommandHSetNX(Module* Creator) : Command(Creator, "HSETNX", 3, 3)
{
         group = 'm';
         syntax = "<map> <key> <value>";
}

COMMAND_RESULT CommandHSetNX::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap  = parameters[0];
       const std::string& key   = parameters[1];
       const std::string& value = parameters.back();

       if (!CheckKey(user, key) || !CheckKey(user, kmap))
       {
            return FAILED;
       }

       if (!CheckFormat(user, value))
       {
            return FAILED;
       }

       MapsHelper::SetNX(user, kmap, key, value);
       return SUCCESS;
}

