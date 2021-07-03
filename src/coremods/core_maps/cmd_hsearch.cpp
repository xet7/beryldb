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
#include "engine.h"
#include "core_maps.h"

CommandHSearch::CommandHSearch(Module* Creator) : Command(Creator, "HSEARCH", 1, 3)
{
         syntax = "<map> <offset> <limit>";
}

COMMAND_RESULT CommandHSearch::Handle(User* user, const Params& parameters)
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
             if (!is_zero_or_great(parameters[1]) || !is_zero_or_great(parameters[2]))
             {
                  user->SendProtocol(ERR_USE, ERR_GREAT_ZERO, MUST_BE_GREAT_ZERO.c_str());
                  return FAILED;
             }

             limit = convto_num<signed int>(parameters[2]); 
             offset = convto_num<signed int>(parameters[1]);
       }
       else
       {
            limit = -1;
            offset = 0;
       }

       //MapsHelper::Search(user, user->current_db, user->select, key, offset, limit);

       return SUCCESS;
}

CommandHSeek::CommandHSeek(Module* Creator) : Command(Creator, "HSEEK", 1, 3)
{
         syntax = "<hash> <offset> <limit>";
}

COMMAND_RESULT CommandHSeek::Handle(User* user, const Params& parameters)
{  
       const std::string& hesh = parameters[0];

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
             if (!is_zero_or_great(parameters[1]) || !is_zero_or_great(parameters[2]))
             {
                  user->SendProtocol(ERR_USE, ERR_GREAT_ZERO, MUST_BE_GREAT_ZERO.c_str());
                  return FAILED;
             }

             limit = convto_num<signed int>(parameters[2]); 
             offset = convto_num<signed int>(parameters[1]);
       }
       else
       {
            limit = -1;
            offset = 0;
       }

       //MapsHelper::SearchHesh(user, user->current_db, user->select, hesh, offset, limit);

       return SUCCESS;
}

CommandHKeys::CommandHKeys(Module* Creator) : Command(Creator, "HKEYS", 1, 3)
{
         syntax = "<map> <offset> <limit>";
}

COMMAND_RESULT CommandHKeys::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

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

       MapsHelper::HKeys(user, key, offset, limit);
       return SUCCESS;
}