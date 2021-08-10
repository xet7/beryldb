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

#include "channelmanager.h"
#include "modules/message.h"

/* 
 * Subscribes an user to a channel. 
 * 
 * @parameters:
 *
 *         · string     : Channel(s) to join.
 * 
 * @protocol:
 *
 *         · enum       : PROCESS_ALREADY, OK or ERROR.
 */ 

class CommandJoin : public MultiCommand
{
 public:
	
	CommandJoin(Module* parent);

	COMMAND_RESULT HandleLocal(LocalUser* user, const Params& parameters);
};

/* 
 * Returns list containing all users in a given channel.
 * 
 * @parameters:
 *
 *         · string     : Channel(s) to slist.
 * 
 * @protocol:
 *
 *         · enum	: NULL.
 *         · vector     : Clients subscribed to a channel.
 */ 

class CommandSlist : public MultiCommand
{
  private:
 
	Events::ModuleEventProvider namesevprov;

  public:
	
	CommandSlist(Module* parent);

	COMMAND_RESULT HandleLocal(LocalUser* user, const Params& parameters);
	
	void SendSlist(LocalUser* user, Channel* chan);
};

/* Unsubscribes an user from a channel. */

class CommandPart : public Command
{
  public:

        MessageWrapper msgwrap;

        CommandPart(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
        RouteParams GetRouting(User* user, const Params& parameters);
};

/* 
 * Returns a list of channels where you are subscribed.
 * 
 * @protocol:
 *
 *         · list      : My channels.
 */ 
 
class CommandMyChans : public Command
{
  public:

        CommandMyChans(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandChans : public Command
{
 public:

        CommandChans(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Unsubscribes from all channels in one command.
 * 
 * @protocol:
 *
 *         · enum       : NO_CHANS or OK.
 */ 

class CommandPartAll : public Command
{
   public:

        CommandPartAll(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
