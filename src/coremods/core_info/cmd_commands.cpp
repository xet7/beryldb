/*
 * BerylDB - A modular database.
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
#include "core_info.h"

CommandSyntax::CommandSyntax(Module* parent) : Command(parent, "SYNTAX", 1, 1)
{
	syntax = "<command>";
}

COMMAND_RESULT CommandSyntax::Handle(User* user, const Params& parameters)
{
	const std::string& cmd = parameters[0];
	
	Command* Found = Kernel->Commander.GetBase(cmd);
	
	if (!Found)
	{
		user->SendProtocol(ERR_CMD_NOFND, cmd, CMD_NOT_FOUND.c_str());
		return FAILED;
	}
	
	user->SendProtocol(BRLD_SYNTAX, Found->name, Found->syntax);
	return SUCCESS;
}

CommandCommands::CommandCommands(Module* parent) : Command(parent, "COMMANDS", 0, 0)
{

}

COMMAND_RESULT CommandCommands::Handle(User* user, const Params& parameters)
{
	const CommandHandler::CommandMap& commands = Kernel->Commander.GetCommands();
	
	std::vector<std::string> list;
	list.reserve(commands.size());

	for (CommandHandler::CommandMap::const_iterator i = commands.begin(); i != commands.end(); ++i)
	{
		/* Requesting user must have required flags. */
		
		if (i->second->requires)
		{
			if (user->CanPerform(i->second->requires))
			{
	                        list.push_back(Daemon::Format("%s", i->second->name.c_str()));
			}
			else
			{
				/* User does not have required flags. */
				
				continue;
			}
		}
		else
		{
			list.push_back(Daemon::Format("%s", i->second->name.c_str()));
		}
	}

	std::sort(list.begin(), list.end());
	
	for (unsigned int i = 0; i < list.size(); i++)
	{
		user->SendProtocol(BRLD_COMMANDS, list[i]);
	}
	
	user->SendProtocol(BRLD_COMMANDSEND, "End of COMMANDS list");
	return SUCCESS;
}
