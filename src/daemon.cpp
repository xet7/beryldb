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
#include "exit.h"

const char* ExitMap[] =
{
           "No errors took place",
           "Core error",
           "Config file error",
           "Logfile error",
           "POSIX fork failed",
           "Bad commandline parameters",
           "Unable to write PID file",
           "Problem with SocketPool",
           "Refusing to start up as root",
           "Couldn't load module on startup",
           "Received SIGTERM",
           "Database error",
           "SIG INT received",
           "Shutdown command received"
};

unsigned const char *locale_case_insensitive_map = brld_case_insensitive_map;

void Beryl::RandomSeed()
{
	timespec current = Kernel->TIME;

#if !defined ARC4RANDOM_BUF_OK
        srandom(current.tv_nsec ^ current.tv_sec);
#endif
}

void Beryl::Refresh()
{
#if defined CLOCK_GETTIME_OK

	clock_gettime(CLOCK_REALTIME, &TIME);

#else

	struct timeval tv;
	gettimeofday(&tv, NULL);

	TIME.tv_sec = tv.tv_sec;
	TIME.tv_nsec = tv.tv_usec * 1000;

#endif
}

bool Daemon::LoginValidator(const std::string& login)
{
	/* Basic length validators. */
	
	if (login.empty() || login.length() < 3 || login.length() > 15)
	{
		return false;
	}

	for (std::string::const_iterator i = login.begin(); i != login.end(); ++i)
	{
		if ((*i >= 'A') && (*i <= '}'))
		{
			continue;
		}

		if ((((*i >= '0') && (*i <= '9')) || (*i == '-')) && (i != login.begin()))
		{
			continue;
		}

		return false;
	}

	return true;
}

bool Daemon::AgentValidator(const std::string& agent)
{
	if (agent.empty() || agent.length() < 3 || agent.length() > 15)
	{
		return false;
	}

	for (std::string::const_iterator i = agent.begin(); i != agent.end(); ++i)
	{
		if ((*i >= 'A') && (*i <= '}'))
		{
			continue;
		}

		if (((*i >= '0') && (*i <= '9')) || (*i == '-') || (*i == '.'))
		{
			continue;
		}

		return false;
	}

	return true;
}

bool Daemon::ValidHost(const std::string& host)
{
	if (host.empty() || host.length() > 96)
	{
		return false;
	}

	unsigned int dash_count = 0;
	unsigned int dots_count = 0;
	bool dots_counted = false;
	const std::string::const_iterator hostend = host.end() - 1;
	
	for (std::string::const_iterator iter = host.begin(); iter != host.end(); ++iter)
	{
		unsigned char chr = static_cast<unsigned char>(*iter);

		if (chr == '.')
		{
			dots_count++;
			
			if (dots_counted || dash_count || iter == host.begin() || iter == hostend)
			{
				return false;
			}

			dots_counted = true;
			continue;
		}

		dots_counted = false;

		if (chr == '-')
		{
			if (dots_counted || dash_count >= 2 || iter == host.begin() || iter == hostend)
			{
				return false;
			}

			dash_count += 1;
			continue;
		}
		
		dash_count = 0;

		if ((chr >= '0' && chr <= '9') || (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z'))
		{
			continue;
		}

		return false;
	}

	return dots_count;
}

bool Daemon::Valid_SID(const std::string &str)
{
	return ((str.length() == 3) && isdigit(str[0]) && ((str[1] >= 'A' && str[1] <= 'Z') || isdigit(str[1])) && ((str[2] >= 'A' && str[2] <= 'Z') || isdigit(str[2])));
}

static const unsigned int num_duration[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 86400, 0, 0, 0, 3600, 0, 0, 0, 0, 60, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 604800, 0, 31557600, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 86400, 0, 0, 0, 3600, 0, 0, 0, 0, 60, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 604800, 0, 31557600, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

bool Daemon::Duration(const std::string& str, unsigned long& duration)
{
	unsigned long total = 0;
	unsigned long subtotal = 0;

	for (std::string::const_iterator i = str.begin(); i != str.end(); ++i)
	{
		if ((*i >= '0') && (*i <= '9'))
		{
			subtotal = (subtotal * 10) + (*i - '0');
		}
		else
		{
			unsigned int multiplier = num_duration[static_cast<unsigned char>(*i)];

			if (multiplier == 0)
			{
				return false;
			}

			total += subtotal * multiplier;

			subtotal = 0;
		}
	}

	duration = total + subtotal;
	return true;
}

unsigned long Daemon::Duration(const std::string& str)
{
	unsigned long out = 0;
	Daemon::Duration(str, out);
	return out;
}

bool Daemon::IsValidDuration(const std::string& duration)
{
	for (std::string::const_iterator i = duration.begin(); i != duration.end(); ++i)
	{
		unsigned char c = *i;

		if (((c >= '0') && (c <= '9')))
		{
			continue;
		}

		if (!num_duration[c])
		{
			return false;
		}
	}
	
	return true;
}

std::string Daemon::duration_as_string(time_t duration)
{
	if (duration == 0)
	{
		return "0s";
	}

	time_t years = duration / 31449600;
	time_t weeks = (duration / 604800) % 52;
	time_t days = (duration / 86400) % 7;
	time_t hours = (duration / 3600) % 24;
	time_t minutes = (duration / 60) % 60;
	time_t seconds = duration % 60;

	std::string formatted;

	if (years)
	{
		formatted = convto_string(years) + "y";
	}
	
	if (weeks)
	{
		formatted += convto_string(weeks) + "w";
	}
	
	if (days)
	{
		formatted += convto_string(days) + "d";
	}
	
	if (hours)
	{
		formatted += convto_string(hours) + "h";
	}
	
	if (minutes)
	{
		formatted += convto_string(minutes) + "m";
	}
	
	if (seconds)
	{
		formatted += convto_string(seconds) + "s";
	}

	return formatted;
}

