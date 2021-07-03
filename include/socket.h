
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

#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#include <cerrno>

namespace engine
{
	namespace sockets
	{
		union ExportAPI sockaddrs
		{
			struct sockaddr sa;
			struct sockaddr_in in4;
			struct sockaddr_in6 in6;
			struct sockaddr_un un;
			
			int family() const;
			
			socklen_t sa_size() const;
			
			int port() const;
			
			std::string addr() const;
			
			std::string str() const;
			bool operator==(const sockaddrs& other) const;
			inline bool operator!=(const sockaddrs& other) const { return !(*this == other); }
		};

		struct ExportAPI cidr_mask
		{
			
			unsigned char type;
			
			unsigned char length;
			
			unsigned char bits[16];

			cidr_mask() {}
			
			cidr_mask(const std::string& mask);
			
			cidr_mask(const engine::sockets::sockaddrs& addr, unsigned char len);
			
			bool operator==(const cidr_mask& other) const;
			
			bool operator<(const cidr_mask& other) const;
			
			bool match(const engine::sockets::sockaddrs& addr) const;
			
			std::string str() const;
		};

		
		ExportAPI bool MatchCompactIP(const std::string &address, const std::string &cidr_mask, bool check_agent);

		
		ExportAPI bool aptosa(const std::string& addr, int port, engine::sockets::sockaddrs& sa);

		
		ExportAPI bool untosa(const std::string& path, engine::sockets::sockaddrs& sa);

		
		ExportAPI bool isunix(const std::string& file);
	}
}


struct ExportAPI FailedPort
{
	
	int error;

	
	engine::sockets::sockaddrs sa;

	
	config_rule* tag;

	FailedPort(int err, engine::sockets::sockaddrs& ep, config_rule* cfg)
		: error(err)
		, sa(ep)
		, tag(cfg)
	{
	}
};

typedef std::vector<FailedPort> FailedListenings;

#include "socketstream.h"


class ExportAPI BindingPort : public EventHandler
{
 public:
	reference<config_rule> listen_tag;
	const engine::sockets::sockaddrs bind_sa;

	class IOQueueProvRef : public ReferencedFrom_nocheck<QueueProvider>
	{
	 public:

		IOQueueProvRef() : ReferencedFrom_nocheck<QueueProvider>(NULL, std::string())
		{
		
		}
	};

	typedef STR1::array<IOQueueProvRef, 2> IOQueueProvList;

	
	IOQueueProvList ioattachprovs;

	
	BindingPort(config_rule* tag, const engine::sockets::sockaddrs& listen_to);
	
	~BindingPort();

	void OnPendingRead() ;

	void ResetQueueProvider();
};
