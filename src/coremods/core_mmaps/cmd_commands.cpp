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
#include "core_mmaps.h"

CommandMGet::CommandMGet(Module* Creator) : Command(Creator, "MGET", 1, 3)
{
        check_key       =       0;
        group 		= 	'x';
        syntax 		= 	"<map> <limit> <offset>";
}

COMMAND_RESULT CommandMGet::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap 		= 	parameters[0];
       Limiter conf = GetLimits(user, this->max_params, parameters);

       if (conf.error)
       {
            return FAILED; 
       }

       KeyHelper::RetroLimits(user, std::make_shared<mget_query>(), kmap, conf.offset, conf.limit);
       return SUCCESS;
}

CommandMCount::CommandMCount(Module* Creator) : Command(Creator, "MCOUNT", 1, 1)
{
        check_key       =       0;
        group 		= 	'x';
        syntax 		= 	"<map>";
}

COMMAND_RESULT CommandMCount::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap = parameters[0];

       std::shared_ptr<mget_query> query = std::make_shared<mget_query>();
       query->flags = QUERY_FLAGS_COUNT;

       KeyHelper::Retro(user, query, kmap);
       return SUCCESS;
}

CommandMPush::CommandMPush(Module* Creator) : Command(Creator, "MPUSH", 3, 3)
{
       check_key        =       0;
       check_hash       =       1;
       check_value      =       true;
       group 		= 	'x';
       syntax 		= 	"<map> <key> \"value\"";
}

COMMAND_RESULT CommandMPush::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap  	= 	parameters[0];
       const std::string& key   	= 	parameters[1];
       const std::string& value 	= 	parameters.back();

       KeyHelper::SimpleHesh(user, std::make_shared<mset_query>(), kmap, key, value);
       return SUCCESS;
}

CommandMPushNX::CommandMPushNX(Module* Creator) : Command(Creator, "MPUSHNX", 3, 3)
{
       check_key        =       0;
       check_hash       =       1;
       check_value	=	true;
       group 		= 	'v';
       syntax 		= 	"<map> <key> \"value\"";
}

COMMAND_RESULT CommandMPushNX::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap          =       parameters[0];
       const std::string& key           =       parameters[1];
       const std::string& value         =       parameters.back();

       KeyHelper::SimpleHesh(user, std::make_shared<setnx_query>(), kmap, key, value);
       return SUCCESS;
}

CommandMKeys::CommandMKeys(Module* Creator) : Command(Creator, "MKEYS", 1, 3)
{
        check_key       =       0;
        group  		= 	'x';
        syntax 		= 	"<map> <limit> <offset>";
}

COMMAND_RESULT CommandMKeys::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap 		  = 	parameters[0];
       Limiter conf = 	GetLimits(user, this->max_params, parameters);

       if (conf.error)
       {
            return FAILED; 
       }

       KeyHelper::RetroLimits(user, std::make_shared<mkeys_query>(), kmap, conf.offset, conf.limit, true);
       return SUCCESS;
}

CommandMDel::CommandMDel(Module* Creator) : Command(Creator, "MDEL", 2, 2)
{
        check_key       =       0;
        check_hash	=	1;
        group 		= 	'x';
        syntax 		= 	"<map> <key>";
}

COMMAND_RESULT CommandMDel::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap  	= 	parameters[0];
       const std::string& key   	= 	parameters[1];

       KeyHelper::Simple(user, std::make_shared<mdel_query>(), kmap, key);
       return SUCCESS;
}

CommandMRepeats::CommandMRepeats(Module* Creator) : Command(Creator, "MREPEATS", 2, 2)
{
        check_key       =       0;
        check_hash      =       1;
        group 		= 	'x';
        syntax 		= 	"<map> <key>";
}

COMMAND_RESULT CommandMRepeats::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap  	= 	parameters[0];
       const std::string& key 		= 	parameters[1];

       KeyHelper::Simple(user, std::make_shared<mrepeats_query>(), kmap, key);
       return SUCCESS;
}

CommandMVals::CommandMVals(Module* Creator) : Command(Creator, "MVALS", 1, 3)
{
        check_key       =       0;
        group 		= 	'x';
        syntax 		= 	"<map> <offset> <limit>";
}

COMMAND_RESULT CommandMVals::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap 		   =   parameters[0];
       Limiter conf  =   GetLimits(user, this->max_params, parameters);

       if (conf.error)
       {
            return FAILED; 
       }

       KeyHelper::RetroLimits(user, std::make_shared<mvals_query>(), kmap, conf.offset, conf.limit);
       return SUCCESS;
}

CommandMGetAll::CommandMGetAll(Module* Creator) : Command(Creator, "MGETALL", 1, 3)
{
        check_key       =       0;
        group 		= 	'x';
        syntax 		= 	"<map> <offset> <limit>";
}

COMMAND_RESULT CommandMGetAll::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap = parameters[0];
       Limiter conf = GetLimits(user, this->max_params, parameters);

       if (conf.error)
       {
            return FAILED; 
       }

       KeyHelper::RetroLimits(user, std::make_shared<mgetall_query>(), kmap, conf.offset, conf.limit);
       return SUCCESS;
}

CommandMIter::CommandMIter(Module* Creator) : Command(Creator, "MITER", 2, 4)
{
       check_key        =       0;
       check_hash       =       1;
       group  		= 	'x';
       syntax 		= 	"<map> <key> <limit> <offset>";
}

COMMAND_RESULT CommandMIter::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap 		= 	parameters[0];
       const std::string& key 		= 	parameters[1];

       Limiter conf = GetLimits(user, this->max_params, parameters);

       if (conf.error)
       {
            return FAILED; 
       }

       std::shared_ptr<miter_query> query = std::make_shared<miter_query>();
       query->value = key;
       
       KeyHelper::RetroLimits(user, query, kmap, conf.offset, conf.limit);
       return SUCCESS;
}
