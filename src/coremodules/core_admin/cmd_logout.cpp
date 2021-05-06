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
#include "core_admin.h"

CommandLogout::CommandLogout(Module* parent) : Command(parent, "LOGOUT", 2, 2), protoev(parent, name)
{
	requires = 'e';
	syntax = "<instance>[,<instance>]+ :<reason>";
	INTERPRET2(TR_CUSTOM, TR_CUSTOM);
}

class LogoutMessage : public ProtocolTrigger::Message
{
 public:
 
	LogoutMessage(ProtocolTrigger::EventProvider& protoev, User* user, LocalUser* target, const std::string& text, const std::string& hideinstance) : ProtocolTrigger::Message("LOGOUT", NULL)
	{
		if (hideinstance.empty())
		{
			SetSourceUser(user);
		}
		else
		{
			SetSource(hideinstance);
		}

		PushParamRef(target->instance);
		PushParamRef(text);
	}
};

COMMAND_RESULT CommandLogout::Handle(User* user, const Params& parameters)
{
	if (CommandHandler::HasLoop(user, this, parameters, 0))
	{
		return FAILED;
	}

	User* target = Kernel->Clients->FindInstance(parameters[0]);
	
	if (!target)
	{
		user->SendProtocol(Protocols::NoInstance(parameters[0]));
		return FAILED;
	}

	if (IS_LOCAL(user))
	{
		ModuleResult MOD_RESULT;
		UNTIL_RESULT(InstanceLogout, MOD_RESULT, (user, target, parameters[1]));

		if (MOD_RESULT == MOD_RES_STOP)
		{
			return FAILED;
		}

		logout_reason = "Logout (";
	
		if (!hideinstance.empty())
		{
			logout_reason += hideinstance;
		}
		else
		{
			logout_reason += user->instance;
		}

		logout_reason += " (" + parameters[1] + "))";
	}
	else
	{
		logout_reason.assign(parameters[1], 0);
	}


	if (IS_LOCAL(target))
	{
		LocalUser* localu = IS_LOCAL(target);
		LogoutMessage msg(protoev, user, localu, logout_reason, hideinstance);
		ProtocolTrigger::Event logoutevent(protoev, msg);
		localu->Send(logoutevent);

		this->lastuuid.clear();
	}
	else
	{
		this->lastuuid = target->uuid;
	}
	
	Kernel->Clients->Disconnect(target, logout_reason);

	return SUCCESS;
}

RouteParams CommandLogout::GetRouting(User* user, const Params& parameters)
{
	if (lastuuid.empty())
	{
		return ROUTE_LOCALONLY;
	}
	
	return ROUTE_BROADCAST;
}

void CommandLogout::EncodeParameter(std::string& param, unsigned int index)
{
	param = ((index == 0) ? lastuuid : logout_reason);
}
