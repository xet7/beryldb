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

static bool MatchInternal(const unsigned char* str, const unsigned char* mask, unsigned const char* map)
{
	unsigned char* cp = NULL;
	unsigned char* mp = NULL;
	unsigned char* base = (unsigned char*)str;
	unsigned char* wildcard = (unsigned char*)mask;

	while ((*base) && (*wildcard != '*'))
	{
		if ((map[*wildcard] != map[*base]) && (*wildcard != '?'))
		{
			return 0;
		}

		wildcard++;
		base++;
	}

	while (*base)
	{
		if (*wildcard == '*')
		{
			if (!*++wildcard)
			{
				return 1;
			}

			mp = wildcard;
			cp = base+1;
		}
		else
			if ((map[*wildcard] == map[*base]) || (*wildcard == '?'))
			{
				wildcard++;
				base++;
			}
			else
			{
				wildcard = mp;
				base = cp++;
			}
	}

	while (*wildcard == '*')
	{
		wildcard++;
	}

	return !*wildcard;
}

bool Daemon::Match(const std::string& str, const std::string& mask, unsigned const char* map)
{
	if (!map)
	{
		map = locale_case_insensitive_map;
	}

	return MatchInternal((const unsigned char*)str.c_str(), (const unsigned char*)mask.c_str(), map);
}

bool Daemon::Match(const char* str, const char* mask, unsigned const char* map)
{
	if (!map)
	{
		map = locale_case_insensitive_map;
	}

	return MatchInternal((const unsigned char*)str, (const unsigned char*)mask, map);
}

bool Daemon::MatchCompactIP(const std::string& str, const std::string& mask, unsigned const char* map)
{
	if (engine::sockets::MatchCompactIP(str, mask, true))
	{
		return true;
	}

	return Daemon::Match(str, mask, map);
}

bool Daemon::MatchCompactIP(const char* str, const char* mask, unsigned const char* map)
{
	if (engine::sockets::MatchCompactIP(str, mask, true))
	{
		return true;
	}

	return Daemon::Match(str, mask, map);
}

bool Daemon::MatchMask(const std::string& masks, const std::string& hostname, const std::string& ipaddr)
{
	engine::space_node_stream maske_list(masks);
	std::string mask;

	while (maske_list.items_extract(mask))
	{
		if (Daemon::Match(hostname, mask, ascii_case_insensitive) || Daemon::MatchCompactIP(ipaddr, mask, ascii_case_insensitive))
		{
			return true;
		}
	}

	return false;
}
