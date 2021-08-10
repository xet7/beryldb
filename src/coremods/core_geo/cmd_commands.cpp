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
#include "core_geo.h"

CommandGeoAddPub::CommandGeoAddPub(Module* Creator) : Command(Creator, "GAPUB", 4, 4)
{
         group = 'g';
         syntax = "<name> <channel>[,<channel>] <longitude> <latitude>";
}

COMMAND_RESULT CommandGeoAddPub::Handle(User* user, const Params& parameters)
{
       const std::string& gname 	= 	parameters[0];
       const std::string& chan 		= 	parameters[1];
       const std::string& latitude 	= 	parameters[2];
       const std::string& longitude 	= 	parameters[3];
       
       if (!is_number(latitude, true) || !is_number(longitude, true))
       {
              user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
              return FAILED;
       }

       if (!ValidLong(convto_num<int>(longitude)))
       {
              user->SendProtocol(ERR_INPUT, INVALID_COORD);
              return FAILED;
       }

       if (!ValidLat(convto_num<int>(latitude)))
       {
              user->SendProtocol(ERR_INPUT, INVALID_COORD);
              return FAILED;
       }

       KeyHelper::AddPub(user, std::make_shared<geoadd_pub_query>(), chan, gname, latitude, longitude);
       return SUCCESS;
}

CommandGeoAdd::CommandGeoAdd(Module* Creator) : Command(Creator, "GEOADD", 3, 3)
{
         group = 'g';
         syntax = "<name> <latitude> <longitude>";
}

COMMAND_RESULT CommandGeoAdd::Handle(User* user, const Params& parameters)
{
       const std::string& gname 	= 	parameters[0];
       const std::string& latitude 	= 	parameters[1];
       const std::string& longitude 	= 	parameters[2];
       
       if (!is_number(latitude, true) || !is_number(longitude, true))
       {
                 user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
                 return FAILED;
       }
       
       if (!ValidLong(convto_num<int>(longitude)))
       {
             user->SendProtocol(ERR_INPUT, INVALID_COORD);
             return FAILED;
       }
       
       if (!ValidLat(convto_num<int>(latitude)))
       {
             user->SendProtocol(ERR_INPUT, INVALID_COORD);
             return FAILED;
       }
       
       KeyHelper::HeshVal(user, std::make_shared<geoadd_query>(), gname, latitude, longitude);
       return SUCCESS;
}

CommandGeoAddNX::CommandGeoAddNX(Module* Creator) : Command(Creator, "GEOADDNX", 3, 3)
{
         group = 'g';
         syntax = "<name> <latitude> <longitude>";
}

COMMAND_RESULT CommandGeoAddNX::Handle(User* user, const Params& parameters)
{
       const std::string& gname         =       parameters[0];
       const std::string& latitude      =       parameters[1];
       const std::string& longitude     =       parameters[2];

       if (!is_number(latitude, true) || !is_number(longitude, true))
       {
                 user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
                 return FAILED;
       }

       if (!ValidLong(convto_num<int>(longitude)))
       {
             user->SendProtocol(ERR_INPUT, INVALID_COORD);
             return FAILED;
       }

       if (!ValidLat(convto_num<int>(latitude)))
       {
             user->SendProtocol(ERR_INPUT, INVALID_COORD);
             return FAILED;
       }

       KeyHelper::HeshVal(user, std::make_shared<geoaddnx_query>(), gname, latitude, longitude);
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

       if (!CheckKey(user, gname))
       {
            return FAILED;
       }
       
       KeyHelper::Retro(user, std::make_shared<geoget_query>(), gname);
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

       KeyHelper::RetroLimits(user, std::make_shared<gkeys_query>(), key, lms[1], lms[2]);
       return SUCCESS;
}

CommandGeoCalc::CommandGeoCalc(Module* Creator) : Command(Creator, "GCALC", 2, 2)
{
         group = 'g';
         syntax = "<name> <name2>";
}

COMMAND_RESULT CommandGeoCalc::Handle(User* user, const Params& parameters)
{  
         const std::string& gname 	= 	parameters[0];
         const std::string& gname2 	= 	parameters[1];
         
         if (!CheckKey(user, gname) || !CheckKey(user, gname2))
         {
              return FAILED;
         }
         
         KeyHelper::Simple(user, std::make_shared<geoget_custom_query>(), gname2, gname);
         return SUCCESS;
}

CommandGeoDistance::CommandGeoDistance(Module* Creator) : Command(Creator, "GDIST", 2, 4)
{
         group = 'g';
         syntax = "<name> <max distance> <offset> <limit>";
}

COMMAND_RESULT CommandGeoDistance::Handle(User* user, const Params& parameters)
{  
       const std::string& gname 	= 	parameters[0];
       const std::string& distance 	= 	parameters[1];

       if (!CheckKey(user, gname))
       {
            return FAILED;
       }

       const std::vector<signed int>& lms = GetLimits(user, this->max_params, parameters);

       if (lms[0] == 0)
       {
            return FAILED; 
       }
       
       std::shared_ptr<geodistance_query> query = std::make_shared<geodistance_query>();
       query->value = distance; 
       
       KeyHelper::RetroLimits(user, query, gname, lms[1], lms[2]);
       return SUCCESS;
}

CommandGeoRemove::CommandGeoRemove(Module* Creator) : Command(Creator, "GREM", 2, 2)
{
         group = 'g';
         syntax = "<name> <dist>";
}

COMMAND_RESULT CommandGeoRemove::Handle(User* user, const Params& parameters)
{  
         const std::string& gname 	= 	parameters[0];
         const std::string& dist 	= 	parameters[1];
         
         KeyHelper::SimpleRetro(user, std::make_shared<georem_query>(), gname, dist);
         return SUCCESS;
}

CommandGeoLoGet::CommandGeoLoGet(Module* Creator) : Command(Creator, "GLOGET", 1, 1)
{
         group = 'g';
         syntax = "<name>";
}

COMMAND_RESULT CommandGeoLoGet::Handle(User* user, const Params& parameters)
{
       const std::string& gname = parameters[0];

       if (!CheckKey(user, gname))
       {
            return FAILED;
       }
       
       KeyHelper::SimpleType(user, std::make_shared<geoget_custom_query>(), gname, QUERY_TYPE_LONG);
       return SUCCESS;
}

CommandGeoLaGet::CommandGeoLaGet(Module* Creator) : Command(Creator, "GLAGET", 1, 1)
{
         group = 'g';
         syntax = "<name>";
}

COMMAND_RESULT CommandGeoLaGet::Handle(User* user, const Params& parameters)
{       
       const std::string& gname = parameters[0];

       if (!CheckKey(user, gname))
       {
            return FAILED;
       }
       
       KeyHelper::SimpleType(user, std::make_shared<geoget_custom_query>(), gname, QUERY_TYPE_LAT);
       return SUCCESS;
}
