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

	COMMAND_RESULT Handle(User* user, const Params& parameters) ;
};


class CommandModules : public ServerTargetCommand
{
   public:

	CommandModules(Module* parent);
	
	COMMAND_RESULT Handle(User* user, const Params& parameters) ;
};


class CommandTime : public ServerTargetCommand
{
 public:
	
	CommandTime(Module* parent);
	
	COMMAND_RESULT Handle(User* user, const Params& parameters) ;
};


class CommandVersion : public Command
{
 public:
	
	CommandVersion(Module* parent);
	
	COMMAND_RESULT Handle(User* user, const Params& parameters) ;
};

class CommandL : public Command
{
 public:

        CommandL(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters) ;
};

class CommandSyntax : public Command
{
 public:

        CommandSyntax(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters) ;
};

