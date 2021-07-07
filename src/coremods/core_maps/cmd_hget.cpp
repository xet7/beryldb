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

CommandHGet::CommandHGet(Module* Creator) : Command(Creator, "HGET", 2, 2)
{
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHGet::Handle(User* user, const Params& parameters)
{  
       const std::string kmap = parameters[0];
       const std::string key = parameters[1];

       MapsHelper::Get(user, kmap, key);
       return SUCCESS;
}

CommandHCount::CommandHCount(Module* Creator) : Command(Creator, "HCOUNT", 1)
{
         syntax = "<map>";
}

COMMAND_RESULT CommandHCount::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap = parameters[0];

       MapsHelper::Count(user, kmap);
       return SUCCESS;
}

CommandHExists::CommandHExists(Module* Creator) : Command(Creator, "HEXISTS", 2, 2)
{
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHExists::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap = parameters[0];
       const std::string& key = parameters[1];

       MapsHelper::Exists(user, kmap, key);
       return SUCCESS;
}

CommandHStrlen::CommandHStrlen(Module* Creator) : Command(Creator, "HSTRLEN", 2, 2)
{
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHStrlen::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap = parameters[0];
       const std::string& key = parameters[1];

       MapsHelper::Strlen(user, kmap, key);
       return SUCCESS;
}

CommandHVals::CommandHVals(Module* Creator) : Command(Creator, "HVALS", 1, 3)
{
         syntax = "<map> <offset> <limit>";
}

COMMAND_RESULT CommandHVals::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap = parameters[0];

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);

       if (lms[0] == 0)
       {
            return FAILED; 
       }

       signed int offset = lms[1];
       signed int limit = lms[2];

       MapsHelper::Vals(user, kmap, offset, limit);
       return SUCCESS;
}

