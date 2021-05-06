/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"
#include "core_channel.h"
#include "modules/slist.h"

CommandSlist::CommandSlist(Module* parent) : MultiCommand(parent, "SLIST", 0, 0), namesevprov(parent, "event/slist")
{
	requires = 'e';
	
	syntax = "[<channel>[,<channel>]+]";
}

COMMAND_RESULT CommandSlist::HandleLocal(LocalUser* user, const Params& parameters)
{
	if (parameters.empty())
	{
		user->SendProtocol(BRLD_SYNTAX, syntax);
		return SUCCESS;
	}

	if (CommandHandler::HasLoop(user, this, parameters, 0))
	{
		return SUCCESS;
	}

        Channel* chan = NULL;

	chan = Kernel->Channels->Find(parameters[0]);

	if (chan)
	{
		SendSlist(user, chan);
		return SUCCESS;
	}
	
	user->SendProtocol(Protocols::NoChannel(parameters[0]));
	return FAILED;
}

void CommandSlist::SendSlist(LocalUser* user, Channel* chan)
{
	Numeric::Builder<' '> reply(user, BRLD_SLIST_REPLY, false, chan->name.size() + 3);
	Numeric::Numeric& numeric = reply.GetNumeric();

	numeric.push(std::string(1, '='));

	numeric.push(chan->name);
	numeric.push(std::string());

	std::string prefixlist;
	std::string instance;
	
	const Channel::SubscriptionMap& members = chan->GetInstances();

	for (Channel::SubscriptionMap::const_iterator i = members.begin(); i != members.end(); ++i)
	{
		Subscription* const memb = i->second;

		prefixlist.clear();
		instance = i->first->instance;

		ModuleResult res;
		UNTIL_RESULT_CUSTOM(namesevprov, Names::EventListener, OnInstanceList, res, (user, memb, prefixlist, instance));
		
		if (res != MOD_RES_STOP)
		{
			reply.Add(prefixlist, instance);
		}
	}

	reply.Flush();
	user->SendProtocol(BRLD_END_OF_SLIST, chan->name, "End of SLIST list.");
}
