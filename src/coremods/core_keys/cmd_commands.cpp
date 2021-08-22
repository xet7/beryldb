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
#include "core_keys.h"

CommandKeys::CommandKeys(Module* Creator) : Command(Creator, "KEYS", 1, 3)
{
       run_conf		=	true; 
       group  		= 	'k';
       syntax 		= 	"<\%key> <offset> <limit>";
}

COMMAND_RESULT CommandKeys::Handle(User* user, const Params& parameters)
{  
       KeyHelper::RetroLimits(user, std::make_shared<keys_query>(), parameters[0], this->offset, this->limit, true);
       return SUCCESS;
}

CommandSearch::CommandSearch(Module* Creator) : Command(Creator, "SEARCH", 1, 3)
{
         run_conf	=	true;
         group  	= 	'k';
         syntax 	= 	"<\%key> <offset> <limit>";
}

COMMAND_RESULT CommandSearch::Handle(User* user, const Params& parameters)
{  
       KeyHelper::RetroLimits(user, std::make_shared<search_query>(), parameters[0], this->offset, this->limit, true);
       return SUCCESS;
}

CommandRKey::CommandRKey(Module* Creator) : Command(Creator, "RKEY", 0, 0)
{
         group = 'k';
}

COMMAND_RESULT CommandRKey::Handle(User* user, const Params& parameters)
{
       KeyHelper::Quick(user, std::make_shared<random_query>());
       return SUCCESS;
}
