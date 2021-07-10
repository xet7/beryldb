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


CommandILogin::CommandILogin(Module* parent) : Command(parent, "ILOGIN", 3, 3)
{
	pre_reg_ok = true;
	syntax = "<agent> <auth> <login>";
}

COMMAND_RESULT CommandILogin::Handle(User* user, const Params& parameters)
{
	if (user->logged)
	{
		user->SendProtocol(ERR_ALREADY_LOGGED, ALREADY_LOGGED);
		return FAILED;
	}
	
        const std::string& agent = parameters[0];
        const std::string& auth = parameters[1];
        const std::string& login = parameters[2];
        
        std::string AGENT_CMD = "AGENT";
        std::string AUTH_CMD = "AUTH";
        std::string LOGIN_CMD = "LOGIN";
        
	CommandModel::Params params;
        CommandModel::Params params2;
        CommandModel::Params params3;
	
	params.push_back(agent);
        params2.push_back(auth);
        params3.push_back(login);

	LocalUser* luser = IS_LOCAL(user);
	
        Kernel->Commander->Queue->Add(luser, AGENT_CMD, params);
        Kernel->Commander->Queue->Add(luser, AUTH_CMD, params2);
        Kernel->Commander->Queue->Add(luser, LOGIN_CMD, params3);
        return SUCCESS;
}