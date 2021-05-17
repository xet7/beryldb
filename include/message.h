/*
 * BerylDB - A modular database.
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

class Externalize MessageLoad
{
  public:

	bool echo;


	bool should_update_idle;

	DiscardList exemptions;

	const std::string original_text;

	const ProtocolTrigger::TagMap tags_in;

	ProtocolTrigger::TagMap tags_out;

	std::string text;

 protected:

	MessageLoad(const std::string& msg, const ProtocolTrigger::TagMap& tags)
		: echo(true)
		, should_update_idle(true)
		, original_text(msg)
		, tags_in(tags)
		, text(msg)
	{
	
	}
};

class Externalize MessageTarget
{
 public:


	enum TargetType
	{
		TYPE_INSTANCE,

		TYPE_CHANNEL,

		TYPE_SERVER
	};

 private:

	void* dest;

 public:
	
	char status;

	MessageTarget::TargetType type;

	MessageTarget(Channel* channel, char statusid)
		: dest(channel)
		, status(statusid)
		, type(TYPE_CHANNEL)
	{

	}

	MessageTarget(User* user)
		: dest(user)
		, status(0)
		, type(TYPE_INSTANCE)
	{
	
	}

	
	MessageTarget(std::string* server)
		: dest(server)
		, status(0)
		, type(TYPE_SERVER)
	{
	
	}

	template<typename T> T* Get() const
	{
		return static_cast<T*>(dest);
	}

	const std::string& GetName() const
	{
		switch (type)
		{
			case TYPE_CHANNEL:
				return Get<Channel>()->name;
			case TYPE_INSTANCE:
				return Get<User>()->instance;
			case TYPE_SERVER:
				return *Get<std::string>();
		}

		static const std::string target = "*";
		return target;
	}
};
