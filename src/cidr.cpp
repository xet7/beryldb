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

bool engine::sockets::MatchCompactIP(const std::string &address, const std::string &cidr_mask, bool check_agent)
{
	std::string address_copy;
	std::string cidr_copy;
	
	if (check_agent)
	{
		
		std::string::size_type agent_mask_pos = cidr_mask.rfind('@');
		std::string::size_type agent_addr_pos = address.rfind('@');

		if (agent_mask_pos != std::string::npos && agent_addr_pos != std::string::npos)
		{
			

			return (Daemon::Match(address.substr(0, agent_addr_pos), cidr_mask.substr(0, agent_mask_pos), ascii_case_insensitive) &&
					MatchCompactIP(address.substr(agent_addr_pos + 1), cidr_mask.substr(agent_mask_pos + 1), false));
		}
		else
		{
			address_copy.assign(address, agent_addr_pos + 1, std::string::npos);
			cidr_copy.assign(cidr_mask, agent_mask_pos + 1, std::string::npos);
		}
	}
	else
	{
		address_copy.assign(address);
		cidr_copy.assign(cidr_mask);
	}

	const std::string::size_type per_pos = cidr_copy.rfind('/');

	if ((per_pos == std::string::npos) || (per_pos == cidr_copy.length()-1) || (cidr_copy.find_first_not_of("0123456789", per_pos+1) != std::string::npos) || (cidr_copy.find_first_not_of("0123456789abcdefABCDEF.:") < per_pos))
	{
		return false;
	}

	engine::sockets::sockaddrs addr;

	if (!engine::sockets::aptosa(address_copy, 0, addr))
	{
		return false;
	}

	engine::sockets::cidr_mask mask(cidr_copy);
	engine::sockets::cidr_mask mask2(addr, mask.length);

	return mask == mask2;
}
