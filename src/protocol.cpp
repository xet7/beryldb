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

ProtocolTrigger::Serializer::Serializer(Module* mod, const char* Name) : DataProvider(mod, std::string("serializer/") + Name), evprov(mod)
{

}

bool ProtocolTrigger::Serializer::HandleTag(LocalUser* user, const std::string& tagname, std::string& tagvalue, TagMap& tags) const
{
	if (tagname.empty())
	{
		return false;
	}

	const ::Events::ModuleEventProvider::SubscriberList& list = evprov.GetSubscribers();

	for (::Events::ModuleEventProvider::SubscriberList::const_iterator i = list.begin(); i != list.end(); ++i)
	{
		MessageDataProvider* const tagprov = static_cast<MessageDataProvider*>(*i);
		const ModuleResult res = tagprov->OnProcessTag(user, tagname, tagvalue);

		if (res == MOD_RES_OK)
		{
			return tags.insert(std::make_pair(tagname, MessageProvider(tagprov, tagvalue))).second;
		}
		else if (res == MOD_RES_STOP)
		{
			break;
		}
	}
	
	return true;
}

ProtocolTrigger::TagSelection ProtocolTrigger::Serializer::MakeTagWhitelist(LocalUser* user, const TagMap& tagmap) const
{
	TagSelection tagwl;
	
	for (TagMap::const_iterator i = tagmap.begin(); i != tagmap.end(); ++i)
	{
		const MessageProvider& tagdata = i->second;
	
		if (tagdata.tagprov->ShouldSendTag(user, tagdata))
		{
			tagwl.Select(tagmap, i);
		}
	}
	
	return tagwl;
}

const ProtocolTrigger::SerializedMessage& ProtocolTrigger::Serializer::SerializeForUser(LocalUser* user, Message& msg)
{
	if (!msg.msginit_done)
	{
		msg.msginit_done = true;
		NOTIFY_MODS_CUSTOM(evprov, MessageDataProvider, OnPopulateTags, (msg));
	}
	
	return msg.GetSerialized(Message::SerializedData(this, MakeTagWhitelist(user, msg.GetTags())));
}

const ProtocolTrigger::SerializedMessage& ProtocolTrigger::Message::GetSerialized(const SerializedData& serializeinfo) const
{
	for (SerializedList::const_iterator i = serlist.begin(); i != serlist.end(); ++i)
	{
		const SerializedData& curr = i->first;
	
		if (curr == serializeinfo)
		{
			return i->second;
		}
	}

	serlist.push_back(std::make_pair(serializeinfo, serializeinfo.serializer->Serialize(*this, serializeinfo.tagwl)));
	return serlist.back().second;
}

void ProtocolTrigger::Event::get_messagesForUser(LocalUser* user, MessageList& messagelist)
{
	if (!eventinit_done)
	{
		eventinit_done = true;
		NOTIFY_MODS_CUSTOM(*event, EventHook, OnEventInit, (*this));
	}

	if (initialmsg)
	{
		messagelist.assign(1, initialmsg);
	}
	else
	{
		messagelist = *initialmsglist;
	}

	ModuleResult res;
	UNTIL_RESULT_CUSTOM(*event, EventHook, OnPreEventSend, res, (user, *this, messagelist));
	
	if (res == MOD_RES_STOP)
	{
		messagelist.clear();
	}
}

Serializable::Data& Serializable::Data::Load(const std::string& key, std::string& out)
{
	EntryMap::iterator iter = this->entries.find(key);

	if (iter == this->entries.end())
	{

	}
	else
	{
		out = iter->second;
	}
	
	return *this;
}

Serializable::Data& Serializable::Data::Load(const std::string& key, Serializable::Data& out)
{
	ChildMap::iterator iter = this->children.find(key);

	if (iter == this->children.end())
	{
		slog("SERIALIZE", LOG_DEBUG, "Unable to load missing data %s!", key.c_str());
	}
	else
	{
		out = iter->second;
		slog("SERIALIZE", LOG_DEBUG, "Loaded data: %s", key.c_str());
	}

	return *this;
}

Serializable::Data& Serializable::Data::Store(const std::string& key, const std::string& value)
{
	this->entries[key] = value;
	return *this;
}

Serializable::Data& Serializable::Data::Store(const std::string& key, const Serializable::Data& value)
{
	this->children[key] = value;
	return *this;
}

bool Expandable::Deserialize(Serializable::Data& data)
{
	if (culled)
		return false;

	const Serializable::Data::EntryMap& entries = data.GetEntries();
	for (Serializable::Data::EntryMap::const_iterator iter = entries.begin(); iter != entries.end(); ++iter)
	{
		const std::string& name = iter->first;
		extension_item* item = Kernel->Extensions.GetItem(name);
		if (item)
		{
			item->from_internal(this, iter->second);
			continue;
		}

		slog("SERIALIZE", LOG_DEBUG, "Tried to deserialize the %s extension item but it doesn't exist",
			name.c_str());
	}
	return true;
}

bool Expandable::Serialize(Serializable::Data& data)
{
	if (culled)
	{
		return false;
	}

	for (Expandable::ExpandableCore::const_iterator iter = extensions.begin(); iter != extensions.end(); ++iter)
	{
		extension_item* item = iter->first;
		const std::string value = item->to_internal(this, iter->second);

		if (!value.empty())
		{
			data.Store(item->name, value);
		}
	}
	
	return true;
}

bool User::Deserialize(Serializable::Data& data)
{
	if (IsQuitting())
	{
		return false;
	}

	std::string client_uuid;
	data.Load("uuid", client_uuid);

	if (!client_uuid.empty() && client_uuid != uuid)
	{
		return false;
	}

	Serializable::Data exts;
	data.Load("extensions", exts);

	if (!Expandable::Deserialize(exts))
	{
		return false;
	}

	long client_port;
	std::string client_addr;

	data.Load("age", age)

		.Load("client_sa.addr", client_addr)
		.Load("client_sa.port", client_port)
		.Load("agent", agent)
		.Load("instance", instance)
		.Load("realhost", realhost)
		.Load("connected", connected);


	engine::sockets::sockaddrs sa;
	
	if (engine::sockets::aptosa(client_addr, client_port, sa) || engine::sockets::untosa(client_addr, sa))
	{
		client_sa = sa;
	}

	ResetCache();
	return true;
}

bool User::Serialize(Serializable::Data& data)
{
	if (IsQuitting())
	{
		return false;
	}

	if (registered != REG_OK)
	{
		return false;
	}

	Serializable::Data exts;
	
	if (!Expandable::Serialize(exts))
	{
		return false;
	}
	
	data.Store("extensions", exts);

	data.Store("age", age)
		.Store("client_sa.addr", client_sa.addr())
		.Store("client_sa.port", client_sa.port())
		.Store("agent", agent)
		.Store("instance", instance)
		.Store("realhost", realhost)
		.Store("connected", connected)
		.Store("uuid", uuid);

	return true;
}

bool LocalUser::Deserialize(Serializable::Data& data)
{

	if (!User::Deserialize(data))
	{
		return false;
	}

	bool user_exempt;
	bool user_lastping;
	long server_port;
	std::string server_addr;

	data.Load("exempt", user_exempt)
		.Load("touchbase", touchbase)
		.Load("lastping", user_lastping)
		.Load("next_ping_check", next_ping_check)
		.Load("server_sa.addr", server_addr)
		.Load("server_sa.port", server_port);

	engine::sockets::sockaddrs sa;
	
	if (engine::sockets::aptosa(server_addr, server_port, sa) || engine::sockets::untosa(server_addr, sa))
	{
		server_sa = sa;
	}

	exempt = user_exempt ? 1 : 0;
	lastping = user_lastping ? 1 : 0;
	return true;
}

bool LocalUser::Serialize(Serializable::Data& data)
{
	if (!User::Serialize(data))
	{
		return false;
	}

	data.Store("exempt", exempt)
		.Store("touchbase", touchbase)
		.Store("lastping", lastping)
		.Store("next_ping_check", next_ping_check)
		.Store("server_sa.addr", server_sa.addr())
		.Store("server_sa.port", server_sa.port());
	return true;
}
