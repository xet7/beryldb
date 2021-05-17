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

#include "beryl.h"

bool Beryl::ListenPort(config_rule* tag, const engine::sockets::sockaddrs& sa)
{
	BindingPort* bport = new BindingPort(tag, sa);

	if (!bport->HasFileDesc())
	{
		delete bport;
		return false;
	}

	binds.push_back(bport);
	return true;
}

size_t Beryl::TryListening(FailedListenings& failed_ports)
{
	size_t bound = 0;

	MultiTag tags = Kernel->Config->GetTags("listen");

	for (config_iterator i = tags.first; i != tags.second; ++i)
	{
		config_rule* tag = i->second;

		const std::string address = tag->as_string("address");
		const std::string portlist = tag->as_string("port");

		if (!address.empty() || !portlist.empty())
		{
			engine::port_template portrange(portlist, false);
			
			for (int port; (port = portrange.items_extract()); )
			{
				engine::sockets::sockaddrs bindspec;

				if (!engine::sockets::aptosa(address, port, bindspec))
				{
					continue;
				}

				if (!ListenPort(tag, bindspec))
				{
					failed_ports.push_back(FailedPort(errno, bindspec, tag));
				}
				else
				{
					bound++;
				}
			}
			
			continue;
		}
	}

	return bound;
}

bool engine::sockets::aptosa(const std::string& addr, int port, engine::sockets::sockaddrs& sa)
{
	memset(&sa, 0, sizeof(sa));

	if (addr.empty() || addr.c_str()[0] == '*')
	{
		if (Kernel->Config->WildcardIPv6)
		{
			sa.in6.sin6_family = AF_INET6;
			sa.in6.sin6_port = htons(port);
		}
		else
		{
			sa.in4.sin_family = AF_INET;
			sa.in4.sin_port = htons(port);
		}

		return true;
	}
	else if (inet_pton(AF_INET, addr.c_str(), &sa.in4.sin_addr) > 0)
	{
		sa.in4.sin_family = AF_INET;
		sa.in4.sin_port = htons(port);
		return true;
	}
	else if (inet_pton(AF_INET6, addr.c_str(), &sa.in6.sin6_addr) > 0)
	{
		sa.in6.sin6_family = AF_INET6;
		sa.in6.sin6_port = htons(port);
		return true;
	}
	return false;
}

bool engine::sockets::untosa(const std::string& path, engine::sockets::sockaddrs& sa)
{
	memset(&sa, 0, sizeof(sa));

	if (path.length() >= sizeof(sa.un.sun_path))
	{
		return false;
	}

	sa.un.sun_family = AF_UNIX;
	memcpy(&sa.un.sun_path, path.c_str(), path.length() + 1);
	return true;
}

bool engine::sockets::isunix(const std::string& file)
{
	struct stat sb;

	if (stat(file.c_str(), &sb) == 0 && S_ISSOCK(sb.st_mode))
	{
		return true;
	}

	return false;
}

int engine::sockets::sockaddrs::family() const
{
	return sa.sa_family;
}

int engine::sockets::sockaddrs::port() const
{
	switch (family())
	{
		case AF_INET:
			return ntohs(in4.sin_port);

		case AF_INET6:
			return ntohs(in6.sin6_port);

		case AF_UNIX:
			return 0;
	}

	return 0;
}

std::string engine::sockets::sockaddrs::addr() const
{
	switch (family())
	{
		case AF_INET:
			char ip4addr[INET_ADDRSTRLEN];
			if (!inet_ntop(AF_INET, (void*)&in4.sin_addr, ip4addr, sizeof(ip4addr)))
				return "0.0.0.0";
			return ip4addr;

		case AF_INET6:
			char ip6addr[INET6_ADDRSTRLEN];
			if (!inet_ntop(AF_INET6, (void*)&in6.sin6_addr, ip6addr, sizeof(ip6addr)))
				return "0:0:0:0:0:0:0:0";
			return ip6addr;

		case AF_UNIX:
			return un.sun_path;
	}

	return "<unknown>";
}

std::string engine::sockets::sockaddrs::str() const
{
	switch (family())
	{
		case AF_INET:
			char ip4addr[INET_ADDRSTRLEN];
			if (!inet_ntop(AF_INET, (void*)&in4.sin_addr, ip4addr, sizeof(ip4addr)))
				strcpy(ip4addr, "0.0.0.0");
			return Daemon::Format("%s:%u", ip4addr, ntohs(in4.sin_port));

		case AF_INET6:
			char ip6addr[INET6_ADDRSTRLEN];
			if (!inet_ntop(AF_INET6, (void*)&in6.sin6_addr, ip6addr, sizeof(ip6addr)))
				strcpy(ip6addr, "0:0:0:0:0:0:0:0");
			return Daemon::Format("[%s]:%u", ip6addr, ntohs(in6.sin6_port));

		case AF_UNIX:
			return un.sun_path;
	}

	return "<unknown>";
}

socklen_t engine::sockets::sockaddrs::sa_size() const
{
	switch (family())
	{
		case AF_INET:
			return sizeof(in4);

		case AF_INET6:
			return sizeof(in6);

		case AF_UNIX:
			return sizeof(un);
	}

	return 0;
}

bool engine::sockets::sockaddrs::operator==(const engine::sockets::sockaddrs& other) const
{
	if (family() != other.family())
	{
		return false;
	}

	switch (family())
	{
		case AF_INET:
			return (in4.sin_port == other.in4.sin_port) && (in4.sin_addr.s_addr == other.in4.sin_addr.s_addr);

		case AF_INET6:
			return (in6.sin6_port == other.in6.sin6_port) && !memcmp(in6.sin6_addr.s6_addr, other.in6.sin6_addr.s6_addr, 16);

		case AF_UNIX:
			return !strcmp(un.sun_path, other.un.sun_path);
	}

	return !memcmp(this, &other, sizeof(*this));
}

static void sa2cidr(engine::sockets::cidr_mask& cidr, const engine::sockets::sockaddrs& sa, unsigned char range)
{
	const unsigned char* base;
	unsigned char target_byte;

	memset(cidr.bits, 0, sizeof(cidr.bits));

	cidr.type = sa.family();
	switch (cidr.type)
	{
		case AF_UNIX:
			cidr.length = 0;
			return;

		case AF_INET:
			cidr.length = range > 32 ? 32 : range;
			target_byte = sizeof(sa.in4.sin_addr);
			base = (unsigned char*)&sa.in4.sin_addr;
			break;

		case AF_INET6:
			cidr.length = range > 128 ? 128 : range;
			target_byte = sizeof(sa.in6.sin6_addr);
			base = (unsigned char*)&sa.in6.sin6_addr;
			break;

		default:
			slog("SOCKET", LOG_DEBUG, "BUG: sa2cidr(): socket type %d is unknown!", cidr.type);
			cidr.length = 0;
			return;
	}

	unsigned int border = cidr.length / 8;
	unsigned int bitmask = (0xFF00 >> (range & 7)) & 0xFF;

	for(unsigned int i=0; i < target_byte; i++)
	{
		if (i < border)
		{
			cidr.bits[i] = base[i];
		}
		else if (i == border)
		{
			cidr.bits[i] = base[i] & bitmask;
		}
		else
		{
			return;
		}
	}
}

engine::sockets::cidr_mask::cidr_mask(const engine::sockets::sockaddrs& sa, unsigned char range)
{
	sa2cidr(*this, sa, range);
}

engine::sockets::cidr_mask::cidr_mask(const std::string& mask)
{
	std::string::size_type bits_chars = mask.rfind('/');
	engine::sockets::sockaddrs sa;

	if (bits_chars == std::string::npos)
	{
		engine::sockets::aptosa(mask, 0, sa);
		sa2cidr(*this, sa, 128);
	}
	else
	{
		unsigned char range = convto_num<unsigned char>(mask.substr(bits_chars + 1));
		engine::sockets::aptosa(mask.substr(0, bits_chars), 0, sa);
		sa2cidr(*this, sa, range);
	}
}

std::string engine::sockets::cidr_mask::str() const
{
	engine::sockets::sockaddrs sa;
	sa.sa.sa_family = type;

	unsigned char* base;
	size_t len;
	
	switch (type)
	{
		case AF_INET:
			base = (unsigned char*)&sa.in4.sin_addr;
			len = 4;
			break;

		case AF_INET6:
			base = (unsigned char*)&sa.in6.sin6_addr;
			len = 16;
			break;

		case AF_UNIX:
			return sa.un.sun_path;

		default:
			slog("SOCKET", LOG_DEBUG, "BUG: engine::sockets::cidr_mask::str(): socket type %d is unknown!", type);
			return "<unknown>";
	}

	memcpy(base, bits, len);
	return sa.addr() + "/" + convto_string((int)length);
}

bool engine::sockets::cidr_mask::operator==(const cidr_mask& other) const
{
	return type == other.type && length == other.length &&
		0 == memcmp(bits, other.bits, 16);
}

bool engine::sockets::cidr_mask::operator<(const cidr_mask& other) const
{
	if (type != other.type)
	{
		return type < other.type;
	}
	
	if (length != other.length)
	{
		return length < other.length;
	}

	return memcmp(bits, other.bits, 16) < 0;
}

bool engine::sockets::cidr_mask::match(const engine::sockets::sockaddrs& addr) const
{
	if (addr.family() != type)
	{
		return false;
	}
	
	engine::sockets::cidr_mask tmp(addr, length);
	return tmp == *this;
}
