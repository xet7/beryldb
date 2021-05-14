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
#include "queues.h"

#include <netinet/tcp.h>

BindingPort::BindingPort(config_rule* tag, const engine::sockets::sockaddrs& listen_to)
	: listen_tag(tag)
	, bind_sa(listen_to)
{
	if (listen_to.family() == AF_UNIX)
	{
		const bool replace = tag->as_bool("replace");

		if (replace && engine::sockets::isunix(listen_to.str()))
		{
			unlink(listen_to.str().c_str());
		}
	}

	fd = socket(listen_to.family(), SOCK_STREAM, 0);

	if (!HasFileDesc())
	{
		return;
	}

#ifdef IPV6_V6ONLY
	
	if (listen_to.family() == AF_INET6)
	{
		std::string addr = tag->as_string("address");
		
		const int enable = (addr.empty() || addr == "*") ? 0 : 1;
		
		setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char *>(&enable), sizeof(enable));
	}
#endif

	if (tag->as_bool("free"))
	{
		socklen_t enable = 1;
#if defined IP_FREEBIND
		setsockopt(fd, SOL_IP, IP_FREEBIND, &enable, sizeof(enable));
#elif defined IP_BINDANY
		setsockopt(fd, IPPROTO_IP, IP_BINDANY, &enable, sizeof(enable));
#elif defined SO_BINDANY
		setsockopt(fd, SOL_SOCKET, SO_BINDANY, &enable, sizeof(enable));
#else
		(void)enable;
#endif
	}

	SocketPool::SetReusable(fd);
	
	int rv = SocketPool::Bind(this->fd, listen_to);
	
	if (rv >= 0)
	{
		rv = SocketPool::Listen(this->fd, 128);
	}

	if (listen_to.family() == AF_UNIX)
	{
		const std::string permissionstr = tag->as_string("permissions");
		unsigned int permissions = strtoul(permissionstr.c_str(), NULL, 8);
	
		if (permissions && permissions <= 07777)
		{
			chmod(listen_to.str().c_str(), permissions);
		}
	}

	int timeout = tag->get_duration("defer", (tag->as_string("ssl").empty() ? 0 : 3));

	if (timeout && !rv)
	{
#if defined TCP_DEFER_ACCEPT
		setsockopt(fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, sizeof(timeout));
#elif defined SO_ACCEPTFILTER
		struct accept_filter_arg afa;
		memset(&afa, 0, sizeof(afa));
		strcpy(afa.af_name, "dataready");
		setsockopt(fd, SOL_SOCKET, SO_ACCEPTFILTER, &afa, sizeof(afa));
#endif
	}

	if (rv < 0)
	{
		int errstore = errno;
		SocketPool::Shutdown(this, 2);
		SocketPool::Close(this->GetDescriptor());
		this->fd = -1;
		errno = errstore;
	}
	else
	{
		SocketPool::NonBlocking(this->fd);
		SocketPool::AddDescriptor(this, Q_REQ_POOL_READ | Q_NO_WRITE);

		this->ResetQueueProvider();
	}
}

BindingPort::~BindingPort()
{
	if (this->HasFileDesc())
	{
		slog("SOCKET", LOG_DEBUG, "Shut down listener on fd %d", this->fd);
		SocketPool::Shutdown(this, 2);

		if (SocketPool::Close(this) != 0)
		{
			slog("SOCKET", LOG_DEBUG, "Failed to cancel listener: %s", strerror(errno));
		}

		if (bind_sa.family() == AF_UNIX && unlink(bind_sa.un.sun_path))
		{
			slog("SOCKET", LOG_DEBUG, "Failed to unlink UNIX socket: %s", strerror(errno));
		}
	}
}

void BindingPort::OnPendingRead()
{
	engine::sockets::sockaddrs client;
	engine::sockets::sockaddrs server(bind_sa);

	socklen_t length = sizeof(client);
	int incoming_socket_descriptor = SocketPool::Accept(this, &client.sa, &length);

	if (incoming_socket_descriptor < 0)
	{
		Kernel->Stats.Refused++;
		return;
	}

	socklen_t sz = sizeof(server);
	
	if (getsockname(incoming_socket_descriptor, &server.sa, &sz))
	{
		slog("SOCKET", LOG_DEBUG, "Unable to obtain peer: %s", strerror(errno));
	}

	if (client.family() == AF_INET6)
	{
		
		static const unsigned char prefix4in6[12] = { 0,0,0,0, 0,0,0,0, 0,0,0xFF,0xFF };
	
		if (!memcmp(prefix4in6, &client.in6.sin6_addr, 12))
		{
			uint16_t sport = client.in6.sin6_port;
			client.in4.sin_family = AF_INET;
			client.in4.sin_port = sport;
			memcpy(&client.in4.sin_addr.s_addr, client.in6.sin6_addr.s6_addr + 12, sizeof(uint32_t));

			sport = server.in6.sin6_port;
			server.in4.sin_family = AF_INET;
			server.in4.sin_port = sport;
			memcpy(&server.in4.sin_addr.s_addr, server.in6.sin6_addr.s6_addr + 12, sizeof(uint32_t));
		}
	}
	else if (client.family() == AF_UNIX)
	{
		strcpy(client.un.sun_path, server.un.sun_path);
	}

	SocketPool::NonBlocking(incoming_socket_descriptor);

	ModuleResult res;
	UNTIL_RESULT(OnAcceptConnection, res, (incoming_socket_descriptor, this, &client, &server));
	
	if (res == MOD_RES_SKIP)
	{
		const std::string type = listen_tag->as_string("type", "clients", 1);
	
		if (stdhelpers::string::equalsci(type, "clients"))
		{
			Kernel->Clients->AddUser(incoming_socket_descriptor, this, &client, &server);
			res = MOD_RES_OK;
		}
	}
	if (res == MOD_RES_OK)
	{
		Kernel->Stats.Accept++;
	}
	else
	{
		Kernel->Stats.Refused++;
		SocketPool::Close(incoming_socket_descriptor);
	}
}

void BindingPort::ResetQueueProvider()
{
	ioattachprovs[0].SetProvider(listen_tag->as_string("attach"));

	for (IOQueueProvList::iterator i = ioattachprovs.begin(); i != ioattachprovs.end()-1; ++i)
	{
		IOQueueProvRef& curr = *i;

		if ((curr) && (!curr->IsMiddle()))
		{
			curr.SetProvider(std::string());
		}
	}

	std::string provname = listen_tag->as_string("ssl");

	if (!provname.empty())
	{
		provname.insert(0, "ssl/");
	}
	
	ioattachprovs.back().SetProvider(provname);
}
