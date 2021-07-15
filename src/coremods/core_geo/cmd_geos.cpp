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
#include "managers/geo.h"
#include "managers/globals.h"
#include "maker.h"
#include "extras.h"
#include "engine.h"
#include "core_geo.h"

CommandGeoAdd::CommandGeoAdd(Module* Creator) : Command(Creator, "GEOADD", 3, 3)
{
         group = 'g';
         syntax = "<name> <longitude> <latitude>";
}

COMMAND_RESULT CommandGeoAdd::Handle(User* user, const Params& parameters)
{
       const std::string& gname = parameters[0];
       const std::string& latitude = parameters[1];
       const std::string& longitude = parameters[2];
       
       if (!is_number(latitude, true) || !is_number(longitude, true))
       {
                 user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
                 return FAILED;
       }
       
       if (!ValidLong(convto_num<int>(longitude)))
       {
             Dispatcher::SmartCmd(user, ERR_USE, ERR_NOT_VALID_COORDINATE, INVALID_COORD);
             return FAILED;
       }
       
       if (!ValidLat(convto_num<int>(latitude)))
       {
             Dispatcher::SmartCmd(user, ERR_USE, ERR_NOT_VALID_COORDINATE, INVALID_COORD);
             return FAILED;
       }
       
       GeoHelper::Add(user, gname, latitude, longitude);
       return SUCCESS;
}

CommandGeoGet::CommandGeoGet(Module* Creator) : Command(Creator, "GEOGET", 1, 1)
{
         group = 'g';
         syntax = "<name>";
}

COMMAND_RESULT CommandGeoGet::Handle(User* user, const Params& parameters)
{
       const std::string& gname = parameters[0];
       
       GeoHelper::Get(user, gname);
       return SUCCESS;
}


CommandGFind::CommandGFind(Module* Creator) : Command(Creator, "GKEYS", 1, 3)
{
         group = 'g';
         syntax = "<\%name> <offset> <limit>";
}

COMMAND_RESULT CommandGFind::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);

       if (lms[0] == 0)
       {
            return FAILED; 
       }

       signed int offset = lms[1];
       signed int limit = lms[2];
       
       GeoHelper::Find(user, key, offset, limit);
       return SUCCESS;
}

CommandGeoCalc::CommandGeoCalc(Module* Creator) : Command(Creator, "GCALC", 2, 2)
{
         group = 'g';
         syntax = "<name> <name2>";
}

COMMAND_RESULT CommandGeoCalc::Handle(User* user, const Params& parameters)
{  
         const std::string& gname = parameters[0];
         const std::string& gname2 = parameters[1];
         
         GeoHelper::Calc(user, gname, gname2);
         return SUCCESS;
}

CommandGeoDistance::CommandGeoDistance(Module* Creator) : Command(Creator, "GDIST", 2, 4)
{
         group = 'g';
         syntax = "<name> <max distance> <offset> <limit>";
}

COMMAND_RESULT CommandGeoDistance::Handle(User* user, const Params& parameters)
{  
       const std::string& gname = parameters[0];
       const std::string& distance = parameters[1];

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);

       if (lms[0] == 0)
       {
            return FAILED; 
       }

       signed int offset = lms[1];
       signed int limit = lms[2];

       GeoHelper::Distance(user, gname, distance, offset, limit);
       return SUCCESS;
}

CommandGeoRemove::CommandGeoRemove(Module* Creator) : Command(Creator, "GREM", 2, 2)
{
         group = 'g';
         syntax = "<name> <dist>";
}

COMMAND_RESULT CommandGeoRemove::Handle(User* user, const Params& parameters)
{  
         const std::string& gname = parameters[0];
         const std::string& dist = parameters[1];
         
         GeoHelper::Remove(user, gname, dist);
         return SUCCESS;
}
