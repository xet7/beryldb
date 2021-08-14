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
#include "core_groups.h"

CommandFlagUpdate::CommandFlagUpdate(Module* Creator) : Command(Creator, "GUPDATE", 2, 2)
{
        check_key       =       0;
        flags 		= 	'r';
        syntax 		= 	"<name> <flag>";
}

COMMAND_RESULT CommandFlagUpdate::Handle(User* user, const Params& parameters)
{
       const std::string& gname 	= 	parameters[0];
       const std::string& flag 		= 	parameters[1];
       
       const std::shared_ptr<Group>& a_group = Kernel->Groups->Find(gname);
       
       if (!a_group)
       {
             user->SendProtocol(ERR_INPUT, PROCESS_NULL);
             return FAILED;
       }
         
       a_group->UpdateFlags(flag);
       user->SendProtocol(BRLD_OK, PROCESS_OK);
       return SUCCESS;
}