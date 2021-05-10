/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"
#include "core_user.h"

CommandAuth::CommandAuth(Module* parent) : MultiCommand(parent, "AUTH", 1, 1)
{
	pre_reg_ok = true;
	syntax = "<password>";
}

COMMAND_RESULT CommandAuth::HandleLocal(LocalUser* user, const Params& parameters)
{
	const std::string& password = parameters[0];
	
	if (password.length() < 3 || password.length() > 100)
	{
		/* Password must be of at least 3 characters */
		
		Kernel->Clients->Disconnect(user, PASS_MIMMAX_LENGTH);	
		return FAILED;
	}
	else 
	{
		user->auth = password;
	}

	return SUCCESS;
}

