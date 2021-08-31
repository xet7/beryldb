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

#include <vector>
#include <map>

#include "brldb/dbnumeric.h"

namespace ProtocolTrigger
{
	class Event;
	class EventProvider;
	class Message;
	class MessageTagEvent;
	class MessageDataProvider;
	class Serializer;

	typedef std::vector<Message*> MessageList;
	typedef std::vector<std::string> ParamList;
	typedef std::string SerializedMessage;

	struct MessageProvider
	{
		MessageDataProvider* tagprov;
		std::string value;
		void* provdata;
		MessageProvider(MessageDataProvider* prov, const std::string& val, void* data = NULL);
	};

	
	typedef brld::flat_map<std::string, MessageProvider, std::greater<std::string> > TagMap;
}

/* A vector of users */

typedef std::vector<User*> UserVector;

/* A map of strings */

typedef std::map<std::string, std::string> DualMap;

/* A multimap of strings. */

typedef std::multimap<std::string, std::string> DualMMap;

/* A vector of string. */

typedef std::vector<std::string> StringVector;

/* List handler */

typedef std::list<std::string> ListMap;

/* A map of strings */

typedef std::map<std::string, std::string> MapMap;

/* A multimap of strings */

typedef std::multimap<std::string, std::string> MultiMap;

/* Handling messages */

enum HANDLER_MSG
{
     HANDLER_MSG_NOT_FOUND = 1,
     HANDLER_MSG_OK = 2,
     HANDLER_MSG_INVALID = 3
};

#include "nodes.h"
#include "base.h"
