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

CommandHKeys::CommandHKeys(Module* Creator) : Command(Creator, "HKEYS", 1, 3)
{
      check_key =       0;
      group 	= 	'm';
      syntax 	=	 "<map> <offset> <limit>";
}

COMMAND_RESULT CommandHKeys::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       Limiter conf = GetLimits(user, this->max_params, parameters);
       
       if (conf.error)
       {
            return FAILED; 
       }
       
       std::shared_ptr<hfind_query> query = std::make_shared<hfind_query>();
       Helpers::make_map(user, query, "", "", true);

       KeyHelper::RetroLimits(user, query, key, conf.offset, conf.limit, true);
       return SUCCESS;
}

CommandHList::CommandHList(Module* Creator) : Command(Creator, "HLIST", 1, 3)
{
      check_key =       0;
      group  	= 	'm';
      syntax	= 	"<map> <offset> <limit>";
}

COMMAND_RESULT CommandHList::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       Limiter conf = GetLimits(user, this->max_params, parameters);
       
       if (conf.error)
       {
            return FAILED; 
       }
       
       KeyHelper::RetroLimits(user, std::make_shared<hlist_query>(), key, conf.offset, conf.limit);
       return SUCCESS;
}