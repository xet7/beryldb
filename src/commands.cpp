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

CommandModel::CommandModel(Module* mod, const std::string& cmd, unsigned int minpara, unsigned int maxpara) : ServiceProvider(mod, cmd, SERVICE_COMMAND)
		, min_params(minpara)
		, max_params(maxpara)
		, last_empty_ok(true)
{

}

CommandModel::~CommandModel()
{

}

void CommandModel::EncodeParameter(std::string& parameter, unsigned int index)
{

}

RouteParams CommandModel::GetRouting(User* user, const Params& parameters)
{
	return ROUTE_LOCALONLY;
}

Command::Command(Module* mod, const std::string& cmd, unsigned int minpara, unsigned int maxpara)
	: CommandModel(mod, cmd, minpara, maxpara)
	, requires(0)
	, force_manual_route(false)
        , syntax("No syntax required.")
	, pre_reg_ok(false)
	, no_hint_until_reg(false)
{

}

Command::~Command()
{
	Kernel->Commander.Remove(this);
}

void Command::RegisterService()
{
	if (!Kernel->Commander.add_command(this))
	{
		throw ModuleException("Command already exists: " + name);
	}
}

void Command::MissingParameters(LocalUser* user, const Params& parameters)
{
        if (user->registered != REG_OK && this->no_hint_until_reg)
        {
                  return;
        }
        
        if (user->agent != DEFAULT_EMERALD)
        {
        	user->SendProtocol(ERR_MISS_PARAMS);
        	return;
        }
        
        if (Kernel->Sets->AsBool("syntaxhints"))
        {
                user->SendProtocol(ERR_MISS_PARAMS, name, "Missing parameters. Syntax:");
        }
        else
        {
                user->SendProtocol(ERR_MISS_PARAMS, name, "Missing parameters.");
        }

	if (Kernel->Sets->AsBool("syntaxhints") && user->registered == REG_OK && syntax.length())
	{
		user->SendProtocol(BRLD_SYNTAX, name, Daemon::Format("%s %s", name.c_str(), syntax.c_str()));
	}
}

void Command::NotLogged(LocalUser* user, const Params& parameters)
{
	user->SendProtocol(ERR_NO_AUTH, name, "You have not logged in.");
}

MultiCommand::MultiCommand(Module* me, const std::string& cmd, unsigned int minpara, unsigned int maxpara) : Command(me, cmd, minpara, maxpara)
{

}

COMMAND_RESULT MultiCommand::Handle(User* user, const Params& parameters)
{
	switch (user->usertype)
	{
		case CLIENT_TYPE_LOCAL:
			return HandleLocal(static_cast<LocalUser*>(user), parameters);

		case CLIENT_TYPE_REMOTE:
			return HandleRemote(static_cast<RemoteUser*>(user), parameters);

		case CLIENT_TYPE_SERVER:
			return HandleServer(static_cast<GlobalUser*>(user), parameters);
	}

	return INVALID;
}

COMMAND_RESULT MultiCommand::HandleLocal(LocalUser* user, const Params& parameters)
{
	return INVALID;
}

COMMAND_RESULT MultiCommand::HandleRemote(RemoteUser* user, const Params& parameters)
{
	return INVALID;
}

COMMAND_RESULT MultiCommand::HandleServer(GlobalUser* user, const Params& parameters)
{
	return INVALID;
}
