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
#include "core_user.h"

CommandAgent::CommandAgent(Module* parent) : MultiCommand(parent, "AGENT", 1, 1)
{
	last_empty_ok = false;
	pre_reg_ok = true;
	syntax = "<agent>";
}

COMMAND_RESULT CommandAgent::HandleLocal(LocalUser* user, const Params& parameters)
{
        const std::string& agent = parameters[0];
	
	if (!(user->registered & REG_USER))
	{
	        if (agent.empty() || agent.length() < 3 || agent.length() > 15)
	        {
                        user->SendProtocol(ERR_INPUT, ERR_BAD_FORMAT);
                        return FAILED;
	        }

		if (!Kernel->Engine->IsAgent(agent))
		{
                        user->SendProtocol(ERR_INPUT, ERR_BAD_FORMAT);
                        return FAILED;
		}
		else
		{
			user->SetAgent(agent);
			user->registered = (user->registered | REG_USER);
		}
	}
	else
	{
		user->SendProtocol(ERR_INPUT, ERR_AGENT_IS_SET);
		return FAILED;
	}

	return CheckRegister(user);
}

COMMAND_RESULT CommandAgent::CheckRegister(LocalUser* user)
{
	if (user->registered == REG_LOGINUSER)
	{
		ModuleResult MOD_RESULT;
		UNTIL_RESULT(OnUserRegister, MOD_RESULT, (user));

		if (MOD_RESULT == MOD_RES_STOP)
		{
			return FAILED;
		}
	}

	return SUCCESS;
}
