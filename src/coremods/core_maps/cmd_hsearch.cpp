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
#include "managers/maps.h"
#include "brldb/query.h"
#include "maker.h"
#include "engine.h"
#include "core_maps.h"

CommandHFind::CommandHFind(Module* Creator) : Command(Creator, "HFIND", 1, 3)
{
         group = 'm';
         syntax = "<map> <offset> <limit>";
}

COMMAND_RESULT CommandHFind::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);
       
       if (lms[0] == 0)
       {
            return FAILED; 
       }
       
       signed int offset = lms[1];
       signed int limit = lms[2];

       MapsHelper::Find(user, key, offset, limit);

       return SUCCESS;
}

CommandHList::CommandHList(Module* Creator) : Command(Creator, "HLIST", 1, 3)
{
         group = 'm';
         syntax = "<map> <offset> <limit>";
}

COMMAND_RESULT CommandHList::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);
       
       if (lms[0] == 0)
       {
            return FAILED; 
       }
       
       signed int offset = lms[1];
       signed int limit = lms[2];

       MapsHelper::List(user, key, offset, limit);
       return SUCCESS;
}