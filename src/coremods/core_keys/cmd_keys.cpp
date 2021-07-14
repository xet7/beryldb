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
#include "managers/keys.h"
#include "engine.h"
#include "core_keys.h"
#include "maker.h"

CommandKeys::CommandKeys(Module* Creator) : Command(Creator, "KEYS", 1, 3)
{
         group = 'k';
         syntax = "<\%key> <offset> <limit>";
}

COMMAND_RESULT CommandKeys::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       if (!CheckFormat(user, key))
       {
            return FAILED;
       }

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);
       
       if (lms[0] == 0)
       {
            return FAILED; 
       }
       
       signed int offset = lms[1];
       signed int limit = lms[2];
       
       KeyHelper::Keys(user, key, offset, limit);
       return SUCCESS;
}

CommandSearch::CommandSearch(Module* Creator) : Command(Creator, "SEARCH", 1, 3)
{
         syntax = "<\%key> <offset> <limit>";
}

COMMAND_RESULT CommandSearch::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckFormat(user, key))
       {
            return FAILED;
       }
       
       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);
       
       if (lms[0] == 0)
       {
            return FAILED; 
       }
       
       signed int offset = lms[1];
       signed int limit = lms[2];

       KeyHelper::Search(user, key, offset, limit);
       return SUCCESS;
}

CommandRKey::CommandRKey(Module* Creator) : Command(Creator, "RKEY", 0, 0)
{

}

COMMAND_RESULT CommandRKey::Handle(User* user, const Params& parameters)
{
       KeyHelper::Random(user);
       return SUCCESS;
}
