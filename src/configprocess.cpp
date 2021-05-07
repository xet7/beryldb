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
#include "stackconf.h"
#include "engine.h"

Configuration::ServerPaths::ServerPaths(config_rule* tag)
	: Config(tag->as_string("configdir", CONFIG_PATH, 1))
	, Data(tag->as_string("datadir", DATA_PATH, 1))
	, Log(tag->as_string("logdir", LOG_PATH, 1))
	, DB(tag->as_string("dbdir", DB_PATH, 1))
	, Module(tag->as_string("moduledir", MODULE_PATH, 1))
	, Runtime(tag->as_string("runtimedir", RUNTIME_PATH, 1))
{

}

static config_rule* crate_empty_rule()
{
	file_config_items* items;
	return config_rule::create("empty", "<auto>", 0, items);
}

Configuration::Configuration() : EmptyTag(crate_empty_rule())
				, Paths(EmptyTag)
				, RawLog(false)
				, printbdebug(true)
{
        ddl_base_ref::reset_all();
}

Configuration::~Configuration()
{
	delete EmptyTag;
}

void Configuration::CrossCheckConnectBlocks()
{
	
	size_t connect_counts = config_data.count("connect");

	if (connect_counts == 0)
	{
		file_config_items* items;
		config_rule* tag = config_rule::create("connect", "<auto>", 0, items);
		(*items)["allow"] = "*";
		config_data.insert(std::make_pair("connect", tag));
		connect_counts = 1;
	}

	Classes.resize(connect_counts);
	std::map<std::string, size_t> names;

	bool try_again = true;
	for(size_t attempts = 0; try_again; attempts++)
	{
		try_again = false;
		MultiTag tags = GetTags("connect");
		size_t i = 0;
		
		for (config_iterator it = tags.first; it != tags.second; ++it, ++i)
		{
			config_rule* tag = it->second;
		
			if (Classes[i])
			{
				continue;
			}

			connect_config* parent = NULL;
			std::string parentName = tag->as_string("parent");
			
			if (!parentName.empty())
			{
				std::map<std::string, size_t>::const_iterator parentIter = names.find(parentName);
				if (parentIter == names.end())
				{
					try_again = true;
					if (attempts >= connect_counts)
						throw KernelException("Could not find parent connect class \"" + parentName + "\" for connect block at " + tag->get_tag_location());
					continue;
				}
				parent = Classes[parentIter->second];
			}

			std::string name = tag->as_string("name");
			std::string mask, typeMask;
			char type;

			if (tag->read_string("allow", mask, false))
			{
				type = CC_ALLOW;
				typeMask = 'a' + mask;
			}
			else if (tag->read_string("deny", mask, false))
			{
				type = CC_DENY;
				typeMask = 'd' + mask;
			}
			else if (!name.empty())
			{
				type = CC_NAMED;
				mask = name;
				typeMask = 'n' + mask;
			}
			else
			{
				throw KernelException("Connects are required to have allow, deny, or name specified at " + tag->get_tag_location());
			}

			if (name.empty())
			{
				name = "undefined-" + convto_string(i);
			}
			else
			{
				typeMask = 'n' + name;
			}

			if (names.find(name) != names.end())
			{
				throw KernelException("Two classes with same name: \"" + name + "\" found.");
			}
			
			names[name] = i;

			connect_config* me = parent ?
				new connect_config(tag, type, mask, *parent) :
				new connect_config(tag, type, mask);

			me->name = name;

			std::string ports = tag->as_string("port");

			if (!ports.empty())
			{
				engine::port_template portrange(ports, false);

				while (int port = portrange.items_extract())
				{
					me->ports.insert(port);
				}
			}

			Classes[i] = me;
		}
	}
}

static std::string GetServerHost()
{
	char hostname[256];
	
	if (gethostname(hostname, sizeof(hostname)) == 0)
	{
		std::string name(hostname);

		if (name.find('.') == std::string::npos)
		{
			name.push_back('.');
		}

		if (name.length() <= Daemon::ValidHost(name))
		{
			return name;
		}
	}
	
	return "server.beryl.dev";
}

void Configuration::Fill()
{
	this->Sets.SetDefaults();

	config_rule* options = GetConf("options");
	config_rule* server = GetConf("server");
        config_rule* settings = GetConf("settings");
	
	if (sid.empty())
	{
		ServerName = server->as_string("name", GetServerHost(), Daemon::ValidHost);
		
		sid = server->as_string("id");

		if (!sid.empty() && !Daemon::Valid_SID(sid))
		{
			throw KernelException(sid + " is invalid sever ID.");
		}

		CaseMapping = options->as_string("casemapping", "brld1459", 1);
		
		if (CaseMapping == "ascii")
		{
			locale_case_insensitive_map = ascii_case_insensitive;
		}
		else if (CaseMapping == "brld1459")
		{
			locale_case_insensitive_map = brld_case_insensitive_map;
		}
		else
		{
			throw KernelException("<options:casemapping> must be set to 'ascii', or 'brld1459'");
		}
	}
	
	MaxClients = settings->as_uint("maxclients", 1500);
	
	Network = server->as_string("network", "Network", 1);
	ModifiedVersion = settings->as_string("customversion");
	c_ipv4_range = GetConf("cidr")->as_uint("ipv4clone", 32, 1, 32);
	c_ipv6_range = GetConf("cidr")->as_uint("ipv6clone", 128, 1, 128);
	Paths = ServerPaths(GetConf("path"));

	std::string deflisten = options->as_string("defaultlisten");

	if (stdhelpers::string::equalsci(deflisten, "ipv4"))
	{
		WildcardIPv6 = false;
	}
	else if (stdhelpers::string::equalsci(deflisten, "ipv6"))
	{
		WildcardIPv6 = true;
	}
	else
	{
		WildcardIPv6 = true;
		int socktest = socket(AF_INET6, SOCK_STREAM, 0);

		if (socktest < 0)
		{
			WildcardIPv6 = false;
		}
		else
		{
			SocketPool::Close(socktest);
		}
	}

}

void Configuration::Load()
{
	parse_stack stack(this);
	
	try
	{
		this->Ready = stack.process_conf_buffer(Kernel->GetConfigFile(), 0);
	}
	catch (KernelException& err)
	{
		this->Ready = false;
		errstr << err.get_reason() << std::endl;
	}
}

void Configuration::SetAll()
{
	this->Ready = true;
	
	try
	{
		Fill();

		CrossCheckConnectBlocks();
	}
	catch (KernelException &ce)
	{
		errstr << ce.get_reason() << std::endl;
	}
	
	this->Ready = errstr.str().empty();
	
	if (this->Ready)
	{
		Kernel->Engine->SavePID(true);
	}

	if (!this->Ready)
	{
		bprint(ERROR, "Errors found in your configuration file.");
		Classes.clear();
	}

	while (errstr.good())
	{
		std::string line;
		getline(errstr, line, '\n');

		if (line.empty())
		{
			continue;
		}
		
		bprint(ERROR, "%s", line.c_str());
	}

	errstr.clear();
	errstr.str(std::string());

	if (!this->Ready)
	{
		Kernel->Exit(EXIT_CODE_CONFIG);
	}

	if (!this->Ready)
	{
		return;
	}
}

config_rule* Configuration::GetConf(const std::string &tag)
{
	MultiTag found = config_data.equal_range(tag);
	
	if (found.first == found.second)
	{
		return EmptyTag;
	}
	
	config_rule* rv = found.first->second;
	found.first++;
	
	if (found.first != found.second)
	{
		slog("CONFIG", LOG_DEFAULT, "Multiple <" + tag + "> tags found; only first will be used " "(first at " + rv->get_tag_location() + "; second at " + found.first->second->get_tag_location() + ")");
	}
	
	return rv;
}

MultiTag Configuration::GetTags(const std::string& tag)
{
	return config_data.equal_range(tag);
}

std::string Configuration::Escape(const std::string& str, bool xml)
{
	std::string escaped;

	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		switch (*it)
		{
			case '"':
				escaped += xml ? "&quot;" : "\"";
				break;
			case '&':
				escaped += xml ? "&amp;" : "&";
				break;
			case '\\':
				escaped += xml ? "\\" : "\\\\";
				break;
			default:
				escaped += *it;
				break;
		}
	}

	return escaped;
}

