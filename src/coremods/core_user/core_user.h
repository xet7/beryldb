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
#include "modules/message.h"

class CommandLogin : public MultiCommand
{
 public:
	
	CommandLogin(Module* parent);
	
	COMMAND_RESULT HandleLocal(LocalUser* user, const Params& parameters) ;
};

class CommandAuth : public MultiCommand
{
 public:
	
	CommandAuth(Module* parent);

	COMMAND_RESULT HandleLocal(LocalUser* user, const Params& parameters) ;

};

class CommandExit : public Command
{
 public:
 
	MessageWrapper msgwrap;
	
	CommandExit(Module* parent);
	
	COMMAND_RESULT Handle(User* user, const Params& parameters);

	RouteParams GetRouting(User* user, const Params& parameters);
};

class CommandAgent : public MultiCommand
{
  public:

	CommandAgent(Module* parent);
	
	COMMAND_RESULT HandleLocal(LocalUser* user, const Params& parameters) ;
	
	static COMMAND_RESULT CheckRegister(LocalUser* user);
};


