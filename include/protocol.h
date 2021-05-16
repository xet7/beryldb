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

#include "nodes.h"

class User;

class ProtocolServer
{
 public:
	
	virtual void SendMetaData(const std::string& key, const std::string& data) = 0;
};

class Externalize ProtocolInterface
{
 public:
	typedef ProtocolServer Server;

	class ServerInfo
	{
	 public:
		std::string servername;
		std::string parentname;
		std::string description;
		unsigned int usercount;
		unsigned int opercount;
		unsigned int latencyms;
	};

	typedef std::vector<ServerInfo> ServerList;

	virtual ~ProtocolInterface() { }

	
	virtual bool SendEncapsulatedData(const std::string& targetmask, const std::string& cmd, const CommandModel::Params& params, User* source = NULL) { return false; }

	
	virtual void BroadcastEncap(const std::string& cmd, const CommandModel::Params& params, User* source = NULL, User* omit = NULL) { }

	
	virtual void SendMetaData(Channel* chan, const std::string& key, const std::string& data) { }

	
	virtual void SendMetaData(User* user, const std::string& key, const std::string& data) { }

	
	virtual void SendMetaData(const std::string& key, const std::string& data) { }

	
	virtual void SendMessage(Channel* target, char status, const std::string& text) 
	{ 
	
	}
	
	virtual void SendMessage(User* target, const std::string& text) 
	{ 
	
	}
	 
	virtual void GetServerList(ServerList& sl) 
	{ 
	
	}
};
