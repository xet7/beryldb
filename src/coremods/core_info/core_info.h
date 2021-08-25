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

#pragma once

#include "beryl.h"
#include "engine.h"

#include "managers/keys.h"
#include "managers/user.h"
#include "managers/databases.h"

class TargetedCommand : public Command
{
 public:

	TargetedCommand(Module* mod, const std::string& Name) : Command(mod, Name)
	{
	
	}

	RouteParams GetRouting(User* user, const Params& parameters) ;
};

/* 
 * Returns all commands available in the system.
 * This command allows wildcards.
 *
 * NOTE: If a given command requires certain flags, it will NOT
 *       be returned to user, unless client satisfy the flag condition.
 *
 * @parameters:
 *
 *         · string  : This parameter is optional. If provided,
 *                     a list of matching commands will be returned.
 *
 * @protocol:
 *
 *         · vector  : List of found commands.
 */ 

class CommandCommands : public Command
{
 public:
	
	CommandCommands(Module* parent);

	COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns all modules loaded in the server.
 *
 * @protocol:
 *
 *         · map  : Modules.
 */ 

class CommandModules : public TargetedCommand
{
   public:

	CommandModules(Module* parent);
	
	COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns all core modules loaded in the server.
 *
 * @protocol:
 *
 *         · map  : Core modules.
 */ 

class CommandCoreModules : public TargetedCommand
{
   public:

        CommandCoreModules(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns current time, as expressed in an human-readable format.
 *
 * @parameters:
 *
 *         · string  : Destination server, if provided.
 *
 * @protocol:
 *
 *         · string  : Current time.
 */ 
 
class CommandTime : public TargetedCommand
{
 public:
	
	CommandTime(Module* parent);
	
	COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns current time, as expressed in an epoch format.
 *
 * @parameters:
 *
 *         · string  : Destination server, if provided.
 *
 * @protocol:
 *
 *         · string  : Current epoch.
 */
 
class CommandEpoch:  public TargetedCommand
{
 public:

        CommandEpoch(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns server's version. Keep in mind that if requesting user has 'e' flags,
 * a full version will be returned.
 *
 * @protocol:
 *
 *         · string  : Current version.
 */

class CommandVersion : public Command
{
 public:
	
	CommandVersion(Module* parent);
	
	COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns information of your connection.
 *
 * @protocol:
 *
 *         · list : information.
 */ 

class CommandL : public Command
{
 public:

        CommandL(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns the syntax of a given command.
 *
 * @parameters:
 *  
 *	   · string	: Command to obtain syntax from.
 *
 * @protocol:
 *
 *         · enum 	: NULL or OK.
 *         · string	: Command.
 */ 

class CommandSyntax : public Command
{
 public:

        CommandSyntax(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns current user in use.
 *
 * @protocol:
 *
 *         · string	: Login utilized.
 */

class CommandWhoami : public Command
{
 public:

        CommandWhoami(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns current agent in use.
 *
 * @protocol:
 *
 *         · string     : Agent utilized.
 */

class CommandAgent : public Command
{
 public:

        CommandAgent(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns the very first found user for a given login.
 *
 * @protocol:
 *
 *         · string      :       Instance.
 */

class CommandFirstOf : public Command
{
 public:

        CommandFirstOf(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns an overview of defined keys in current select.
 *
 * @protocol:
 *
 *         · list	:	Defined keys.
 */

class CommandLS : public Command
{
 public:

        CommandLS(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns a counter of all keys, regardless of type, defined in server.
 *
 * @protocol:
 *
 *         · int	:	counter.
 */

class CommandTotal : public Command
{
 public:

        CommandTotal(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns an overview of defined keys in all select.
 *
 * @protocol:
 *
 *         · list       :       Defined keys.
 */

class CommandGS : public Command
{
 public:

        CommandGS(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns time in which this server was started.
 *
 * @protocol:
 *
 *         · string  : Startup time.
 */

class CommandGetStartup : public Command
{
 public:

        CommandGetStartup(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
