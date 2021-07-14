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

class MessageLoadImpl : public MessageLoad
{
 
 public:

	MessageLoadImpl(const std::string& msg, const ProtocolTrigger::TagMap& tags) : MessageLoad(msg, tags)
	{
	
	}
};

namespace
{
	bool AlertPreEvents(User* source, MessageTarget& msgtarget, MessageLoad& msgdetails)
	{
		ModuleResult modres;
		UNTIL_RESULT(OnInstancePrePublish, modres, (source, msgtarget, msgdetails));

		if (modres == MOD_RES_STOP)
		{
			NOTIFY_MODS(OnInstancePublishBlocked, (source, msgtarget, msgdetails));
			return false;
		}

		if (msgdetails.text.empty())
		{
			source->SendProtocol(ERR_INPUT2, ERR_PUB_NOT_PROVIDED, NOT_DEFINED);
			return false;
		}

		NOTIFY_MODS(OnInstancePublish, (source, msgtarget, msgdetails));
		return true;
	}

	COMMAND_RESULT AlertPostEvent(User* source, const MessageTarget& msgtarget, const MessageLoad& msgdetails)
	{
		LocalUser* lsource = IS_LOCAL(source);

		if (lsource && msgdetails.should_update_idle)
		{
			lsource->touchbase = Kernel->Now();
		}

		NOTIFY_MODS(OnInstancePostPublish, (source, msgtarget, msgdetails));
		return SUCCESS;
	}
}

class CommandMessage : public Command
{
 private:

	COMMAND_RESULT HandleChannelTarget(User* source, const Params& parameters, const char* target)
	{
		Channel* chan = Kernel->Channels->Find(target);
	
		if (!chan)
		{
			source->SendProtocol(Protocols::NoChannel(parameters[0]));
			return FAILED;
		}

		MessageTarget msgtarget(chan, 0);
		MessageLoadImpl msgdetails(parameters[1], parameters.GetTags());
		msgdetails.exemptions.insert(source);

		if (!AlertPreEvents(source, msgtarget, msgdetails))
		{
			return FAILED;
		}

		ProtocolTrigger::Messages::Publish publish(ProtocolTrigger::Messages::Publish::no_replicate, source, chan, msgdetails.text, msgtarget.status);
		publish.AddTags(msgdetails.tags_out);
		publish.SetSideEffect(true);
		chan->Write(Kernel->GetBRLDEvents().publish, publish, msgtarget.status, msgdetails.exemptions);

		return AlertPostEvent(source, msgtarget, msgdetails);
	}

	COMMAND_RESULT HandleServerTarget(User* source, const Params& parameters)
	{
		if (!source->IsAdmin())
		{
			source->SendProtocol(ERR_INPUT2, ERR_NO_FLAGS, ACCESS_DENIED);
			return FAILED;
		}

		std::string servername(parameters[0], 1);

		MessageTarget msgtarget(&servername);
		MessageLoadImpl msgdetails(parameters[1], parameters.GetTags());

		if (!AlertPreEvents(source, msgtarget, msgdetails))
		{
			return FAILED;
		}

		if (Daemon::Match(Kernel->Config->ServerName, servername))
		{
			ProtocolTrigger::Messages::Publish message(ProtocolTrigger::Messages::Publish::no_replicate, source, "$*", msgdetails.text);
			message.AddTags(msgdetails.tags_out);
			message.SetSideEffect(true);
			ProtocolTrigger::Event messageevent(Kernel->GetBRLDEvents().publish, message);

			const ClientManager::LocalList& list = Kernel->Clients.GetLocals();

			for (ClientManager::LocalList::const_iterator i = list.begin(); i != list.end(); ++i)
			{
				LocalUser* luser = *i;

				if (luser->registered != REG_OK || luser == source)
				{
					continue;
				}

				if (!msgdetails.exemptions.count(luser))
				{
					luser->Send(messageevent);
				}
			}
		}
		
		return AlertPostEvent(source, msgtarget, msgdetails);
	}

	COMMAND_RESULT HandleUserTarget(User* source, const Params& parameters)
	{
		User* target;
	
		if (IS_LOCAL(source))
		{
			const char* targetserver = strchr(parameters[0].c_str(), '@');
	
			if (targetserver)
			{
				target = Kernel->Clients->FindInstanceOnly(parameters[0].substr(0, targetserver - parameters[0].c_str()));
				
				if (target && strcasecmp(target->server->GetName().c_str(), targetserver + 1))
				{
					target = NULL;
				}
			}
			else
			{
				target = Kernel->Clients->FindInstanceOnly(parameters[0]);
			}
		}
		else
		{
			target = Kernel->Clients->FindInstance(parameters[0]);
		}

		if (!target || target->registered != REG_OK)
		{
			source->SendProtocol(Protocols::NoInstance(parameters[0]));
			return FAILED;
		}

		MessageTarget msgtarget(target);
		MessageLoadImpl msgdetails(parameters[1], parameters.GetTags());
		
		if (!AlertPreEvents(source, msgtarget, msgdetails))
		{
			return FAILED;
		}

		LocalUser* const localtarget = IS_LOCAL(target);
		
		if (localtarget)
		{
			ProtocolTrigger::Messages::Publish publish(ProtocolTrigger::Messages::Publish::no_replicate, source, localtarget->instance, msgdetails.text);
			publish.AddTags(msgdetails.tags_out);
			publish.SetSideEffect(true);
			localtarget->Send(Kernel->GetBRLDEvents().publish, publish);
		}
		
		return AlertPostEvent(source, msgtarget, msgdetails);
	}

 public:

	CommandMessage(Module* parent) : Command(parent, PUBL_DEF, 2, 2)
	{
		syntax = "<target>[,<target>]+ :<message>";
	}

	
	COMMAND_RESULT Handle(User* user, const Params& parameters) 
	{
		if (CommandHandler::HasLoop(user, this, parameters, 0))
		{
			return SUCCESS;
		}

		if (parameters[1].empty())
		{
			user->SendProtocol(ERR_INPUT2, ERR_PUB_NOT_PROVIDED, NOT_DEFINED);
			return FAILED;
		}

		if (parameters[0][0] == '$')
		{
			return HandleServerTarget(user, parameters);
		}
		
		const char* target = parameters[0].c_str();

		if (!target[0])
		{
			user->SendProtocol(ERR_INPUT2, ERR_NORECIPIENT, PROCESS_NULL);
			return FAILED;
		}

		if (*target == '#')
		{
			return HandleChannelTarget(user, parameters, target);
		}
		
		if (!user->InGroup('c') && !user->IsAdmin())
		{
                        user->SendProtocol(ERR_INPUT2, ERR_NO_FLAGS, ACCESS_DENIED);
                        return FAILED;
		}

		return HandleUserTarget(user, parameters);
	}

	RouteParams GetRouting(User* user, const Params& parameters) 
	{
		if (IS_LOCAL(user))
		{
			return ROUTE_LOCALONLY;
		}
		else
		{
			return ROUTE_MESSAGE(parameters[0]);
		}
	}
};

class CommandSQuery : public MultiCommand
{
 public:
 
	CommandSQuery(Module* Creator) : MultiCommand(Creator, "SQUERY", 2, 2)
	{
		syntax = "<service> :<message>";
	}

	COMMAND_RESULT HandleLocal(LocalUser* user, const Params& parameters) 
	{
		if (parameters[1].empty())
		{
			user->SendProtocol(ERR_INPUT2, ERR_PUB_NOT_PROVIDED, NOT_DEFINED);
			return FAILED;
		}
		
		User* target;
		const char* targetserver = strchr(parameters[0].c_str(), '@');

		if (targetserver)
		{
			target = Kernel->Clients->FindInstanceOnly(parameters[0].substr(0, targetserver - parameters[0].c_str()));
		
			if (target && strcasecmp(target->server->GetName().c_str(), targetserver + 1))
			{
				target = NULL;
			}
		}
		else
		{
			target = Kernel->Clients->FindInstanceOnly(parameters[0]);
		}

		if (!target || target->registered != REG_OK)
		{
			user->SendProtocol(ERR_INPUT2, ERR_NOSUCHSERVICE, parameters[0], NOT_DEFINED);
			return FAILED;
		}
		
		MessageTarget msgtarget(target);
		MessageLoadImpl msgdetails(parameters[1], parameters.GetTags());
		
		if (!AlertPreEvents(user, msgtarget, msgdetails))
		{
			return FAILED;
		}
		
		return AlertPostEvent(user, msgtarget, msgdetails);
	}
};

class ModuleCoreMessage : public Module
{
 private:
 
	CommandMessage cmdpublish;
	CommandSQuery cmdsquery;

 public:
 
	ModuleCoreMessage() : cmdpublish(this) , cmdsquery(this)
	{

	}

	ModuleResult OnInstancePrePublish(User* user, const MessageTarget& target, MessageLoad& details) 
	{
		if (!IS_LOCAL(user) || target.type != MessageTarget::TYPE_CHANNEL)
		{
			return MOD_RES_SKIP;
		}

		Channel* chan = target.Get<Channel>();
		
		if (!user->InGroup('p') && !user->IsAdmin() && !chan->IsSubscribed(user))
		{
			user->SendProtocol(ERR_INPUT2, ERR_CANNOTSENDTOCHAN, NO_EXTERNAL);
			return MOD_RES_STOP;
		}
		
		return MOD_RES_OK;
	}

	Version GetDescription() 
	{
		return Version("Provides the PUBLISH, and SQUERY commands", VF_CORE|VF_BERYLDB);
	}
};

MODULE_LOAD(ModuleCoreMessage)
