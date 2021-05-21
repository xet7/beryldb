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

CommandLogin::CommandLogin(Module* parent) : MultiCommand(parent, "LOGIN", 1, 1)
{
	pre_reg_ok = true;
	no_hint_until_reg = true;
	syntax = "<login>";
}

COMMAND_RESULT CommandLogin::HandleLocal(LocalUser* user, const Params& parameters)
{
	const std::string& newlogin = parameters[0];

	if (newlogin.empty())
	{
		user->SendProtocol(ERR_ALREADY_LOGGED, "No login has been provided.");
		return FAILED;
	}

	if (!Kernel->Engine->ValidLogin(newlogin))
	{
		user->SendProtocol(ERR_INVALID_LOGIN, newlogin, "Erroneous login");
		return FAILED;
	}

	ModuleResult MOD_RESULT;
	UNTIL_RESULT(OnUserPreLogin, MOD_RESULT, (user, newlogin));

	if (MOD_RESULT == MOD_RES_STOP)
	{
		return FAILED;
	}

	if (!user->SetLogin(newlogin))
	{
		return FAILED;
	}

	if (user->registered < REG_LOGINUSER)
	{
		user->registered = (user->registered | REG_LOGIN);
		return CommandAgent::CheckRegister(user);
	}

	return SUCCESS;
}
