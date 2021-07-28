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

class ServerTargetCommand : public Command
{
 public:

	ServerTargetCommand(Module* mod, const std::string& Name) : Command(mod, Name)
	{
	
	}

	RouteParams GetRouting(User* user, const Params& parameters) ;
};


class CommandCommands : public Command
{
 public:
	
	CommandCommands(Module* parent);

	COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandModules : public ServerTargetCommand
{
   public:

	CommandModules(Module* parent);
	
	COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandCoreModules : public ServerTargetCommand
{
   public:

        CommandCoreModules(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandTime : public ServerTargetCommand
{
 public:
	
	CommandTime(Module* parent);
	
	COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandEpoch:  public ServerTargetCommand
{
 public:

        CommandEpoch(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandVersion : public Command
{
 public:
	
	CommandVersion(Module* parent);
	
	COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandL : public Command
{
 public:

        CommandL(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandSyntax : public Command
{
 public:

        CommandSyntax(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandWhoami : public Command
{
 public:

        CommandWhoami(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandAgent : public Command
{
 public:

        CommandAgent(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandFirstOf : public Command
{
 public:

        CommandFirstOf(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLS : public Command
{
 public:

        CommandLS(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandTotal : public Command
{
 public:

        CommandTotal(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGS : public Command
{
 public:

        CommandGS(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGetStartup : public Command
{
 public:

        CommandGetStartup(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
