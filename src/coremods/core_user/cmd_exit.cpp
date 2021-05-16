/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"
#include "core_user.h"

CommandExit::CommandExit(Module* parent) : Command(parent, "EXIT", 0) 
{
	pre_reg_ok = true;
}

COMMAND_RESULT CommandExit::Handle(User* user, const Params& parameters)
{
	std::string exitmsg = "Exitting.";

	user->SendProtocol(BRLD_QUITTING);
	Kernel->Clients->Disconnect(user, exitmsg);

	return SUCCESS;
}

RouteParams CommandExit::GetRouting(User* user, const Params& parameters)
{
	return (IS_LOCAL(user) ? ROUTE_LOCALONLY : ROUTE_BROADCAST);
}
