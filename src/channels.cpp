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

#include "channelmanager.h"

Channel::Channel(const std::string &cname, time_t current) : name(cname), created(current)
{
	if (!Kernel->Channels->ChannelList.insert(std::make_pair(cname, this)).second)
	{
		throw KernelException("Channel already exists: " + cname);
	}
}

Subscription* Channel::AddUser(User* user)
{
	std::pair<SubscriptionMap::iterator, bool> added = subscribedlist.insert(std::make_pair(user, brld::aligned_storage<Subscription>()));
	
	if (!added.second)
	{
		return NULL;
	}

	Subscription* member = new(added.first->second) Subscription(user, this);
	return member;
}

void Channel::DeleteUser(User* user)
{
	SubscriptionMap::iterator it = subscribedlist.find(user);
	
	if (it != subscribedlist.end())
	{
		this->DeleteUser(it);
	}
}

void Channel::DeleteUser(const SubscriptionMap::iterator& subsiter)
{
	Subscription* subs = subsiter->second;
	subs->discard();
	subs->~Subscription();
	subscribedlist.erase(subsiter);
	Kernel->Channels->CheckRemoval(this);
}

Subscription* Channel::GetUser(User* user)
{
	SubscriptionMap::iterator i = subscribedlist.find(user);

	if (i == subscribedlist.end())
	{
		return NULL;
	}
	
	return i->second;
}

Channel* Channel::JoinUser(bool fromconnect, LocalUser* user, std::string cname, bool override)
{
	/* We cannot subscribe an unregistered user. */
	
	if (user->registered != REG_OK)
	{
		return NULL;
	}

	/* Channel name too long. */
	
	if (cname.length() > 15)
	{
		cname.resize(15);
	}

	Channel* chan = Kernel->Channels->Find(cname);

	bool created_by_local = (chan == NULL); 

	if (!chan)
	{
		if (!Kernel->Config->AllowNew)
		{
                        user->SendProtocol(ERR_NOT_ACCEPTING_C, cname, "Not accepting new channels.");
                        return NULL;
		}

		if (override == false)
		{
			ModuleResult MOD_RESULT;
			UNTIL_RESULT(OnUserPreJoin, MOD_RESULT, (user, NULL, cname));

			if (MOD_RESULT == MOD_RES_STOP)
			{
				return NULL; 
			}
		}

		chan = new Channel(cname, Kernel->Now());
                falert(NOTIFY_DEFAULT, "New channel: %s", cname.c_str());
	}
	else
	{
		if (chan->IsSubscribed(user))
		{
			return NULL;
		}

		if (override == false)
		{
			ModuleResult MOD_RESULT;
			UNTIL_RESULT(OnUserPreJoin, MOD_RESULT, (user, chan, cname));

			if (MOD_RESULT == MOD_RES_STOP)
			{
				return NULL;
			}
		}
	}

	chan->ExecuteJoin(fromconnect, user, false, created_by_local);
	return chan;
}

Subscription* Channel::ExecuteJoin(bool fromconnect, User* user, bool bursting, bool created_by_local)
{
	if (IS_SERVER(user))
	{
		return NULL;
	}

	Subscription* memb = this->AddUser(user);
	memb->from_connect = fromconnect;
	
	if (!memb)
	{
		return NULL; 
	}

	user->chans.push_front(memb);

	DiscardList except_list;
	NOTIFY_MODS(OnUserJoin, (memb, bursting, created_by_local, except_list));

	ProtocolTrigger::Events::Join joinevent(memb);
	this->Write(joinevent, 0, except_list, user);

	NOTIFY_MODS(OnPostJoin, (memb));
	return memb;
}

bool Channel::PartUser(User* user)
{
	SubscriptionMap::iterator subsiter = subscribedlist.find(user);

	if (subsiter == subscribedlist.end())
	{
		return false;
	}

	Subscription* memb = subsiter->second;
	DiscardList except_list;
	NOTIFY_MODS(OnUserPart, (memb, except_list));

	ProtocolTrigger::Messages::Part partmsg(memb);
	Write(Kernel->GetBRLDEvents().part, partmsg, 0,  except_list, user);
	
	/* Let's remove user from chan list. */
	
	user->chans.erase(memb);
	this->DeleteUser(subsiter);

	return true;
}

void Channel::Write(ProtocolTrigger::Event& protoev, char status, const DiscardList& except_list, User* joining)
{
	for (SubscriptionMap::iterator i = subscribedlist.begin(); i != subscribedlist.end(); i++)
	{
		LocalUser* user = IS_LOCAL(i->first);

		if ((user) && (!except_list.count(user)))
		{
			user->Send(protoev);
		}
	}
}
