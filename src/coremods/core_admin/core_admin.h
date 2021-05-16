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

class CommandLogout : public Command
{
 private:
 
	std::string lastuuid;
	std::string logout_reason;
	ProtocolTrigger::EventProvider protoev;

 public:

	std::string hideinstance;
	
	CommandLogout(Module* parent);

	COMMAND_RESULT Handle(User* user, const Params& parameters);

	RouteParams GetRouting(User* user, const Params& parameters);

	void EncodeParameter(std::string& param, unsigned int index);
};


class CommandFinger : public Command
{
   public:

	CommandFinger(Module* parent);

	COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLogins : public Command
{
   public:

        CommandLogins(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandShutdown : public Command
{
   public:

        CommandShutdown(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
	
};

class CommandRestart : public Command
{
   public:

        CommandRestart(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
        
};
