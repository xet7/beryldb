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
#include "core_maps.h"

CommandHGet::CommandHGet(Module* Creator) : Command(Creator, "HGET", 2, 2)
{
       check_key        =       0;
       check_hash       =       1;
       group 		= 	'm';
       syntax 		= 	"<map> <key>";
}

COMMAND_RESULT CommandHGet::Handle(User* user, const Params& parameters)
{  
       KeyHelper::HeshRetro(user, std::make_shared<hget_query>(), parameters[0], parameters[1]);
       return SUCCESS;
}

CommandHCount::CommandHCount(Module* Creator) : Command(Creator, "HCOUNT", 1, 1)
{
       check_key        =       0;
       group 		= 	'm';
       syntax 		= 	"<map>";
}

COMMAND_RESULT CommandHCount::Handle(User* user, const Params& parameters)
{  
       std::shared_ptr<hlist_query> query = std::make_shared<hlist_query>();
       query->flags 			  = QUERY_FLAGS_COUNT;

       KeyHelper::Retro(user, query, parameters[0]);
       return SUCCESS;
}

CommandHExists::CommandHExists(Module* Creator) : Command(Creator, "HEXISTS", 2, 2)
{
       check_key        =       0;
       check_hash	=	1;
       group 		= 	'm';
       syntax 		= 	"<map> <key>";
}

COMMAND_RESULT CommandHExists::Handle(User* user, const Params& parameters)
{  
       KeyHelper::HeshRetro(user, std::make_shared<hexists_query>(), parameters[0], parameters[1]);
       return SUCCESS;
}

CommandHStrlen::CommandHStrlen(Module* Creator) : Command(Creator, "HSTRLEN", 2, 2)
{
       check_key        =       0;
       check_hash       =       1;
       group 		= 	'm';
       syntax 		= 	"<map> <key>";
}

COMMAND_RESULT CommandHStrlen::Handle(User* user, const Params& parameters)
{  
       KeyHelper::HeshRetro(user, std::make_shared<hstrlen_query>(), parameters[0], parameters[1]);
       return SUCCESS;
}

CommandHVals::CommandHVals(Module* Creator) : Command(Creator, "HVALS", 1, 3)
{
       run_conf		=	true;
       check_key        =       0;
       group 		= 	'm';
       syntax 		= 	"<map> <offset> <limit>";
}

COMMAND_RESULT CommandHVals::Handle(User* user, const Params& parameters)
{  
       KeyHelper::RetroLimits(user, std::make_shared<hvals_query>(), parameters[0], this->offset, this->limit);
       return SUCCESS;
}

CommandHGetAll::CommandHGetAll(Module* Creator) : Command(Creator, "HGETALL", 1, 3)
{
       run_conf		=	true;
       check_key	=	0;
       group 		= 	'm';
       syntax 		= 	"<map> <offset> <limit>";
}

COMMAND_RESULT CommandHGetAll::Handle(User* user, const Params& parameters)
{  
       KeyHelper::RetroLimits(user, std::make_shared<hgetall_query>(), parameters[0], this->offset, this->limit);
       return SUCCESS;
}

CommandHDel::CommandHDel(Module* Creator) : Command(Creator, "HDEL", 2, 2)
{
       check_key        =       0;
       check_hash       =       1;
       group 		= 	'm';
       syntax 		= 	"<map> <key>";
}

COMMAND_RESULT CommandHDel::Handle(User* user, const Params& parameters)
{  
       KeyHelper::SimpleRetro(user, std::make_shared<hdel_query>(), parameters[0], parameters[1]);
       return SUCCESS;
}

CommandHSet::CommandHSet(Module* Creator) : Command(Creator, "HSET", 3, 3)
{
       check_value      =       true;
       check_key        =       0;
       check_hash       =       1;
       group 		= 	'm';
       syntax 		= 	"<map> <key> \"value\"";
}

COMMAND_RESULT CommandHSet::Handle(User* user, const Params& parameters)
{
       KeyHelper::SimpleHesh(user, std::make_shared<hset_query>(),  parameters[0], parameters[1], parameters.back());
       return SUCCESS;
}

CommandHSetNX::CommandHSetNX(Module* Creator) : Command(Creator, "HSETNX", 3, 3)
{
       check_value      =       true;
       check_key        =       0;
       check_hash       =       1;
       group 		= 	'm';
       syntax 		= 	"<map> <key> \"value\"";
}

COMMAND_RESULT CommandHSetNX::Handle(User* user, const Params& parameters)
{  
       KeyHelper::SimpleHesh(user, std::make_shared<hsetnx_query>(), parameters[0], parameters[1], parameters.back());
       return SUCCESS;
}

