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

#include "beryl.h"

class Serializer : public ProtocolTrigger::Serializer
{

	static const std::string::size_type MAX_CLIENT_MESSAGE_TAG_LENGTH = INPUT_LIMIT;

	static const std::string::size_type MAX_SERVER_MESSAGE_TAG_LENGTH = INPUT_LIMIT;

	static void SerializeTags(const ProtocolTrigger::TagMap& tags, const ProtocolTrigger::TagSelection& tagwl, std::string& line);

 public:

	Serializer(Module* mod) : ProtocolTrigger::Serializer(mod, "brld")
	{
		
	}

 	bool Parse(LocalUser* user, const std::string& line, ProtocolTrigger::ParseOutput& parseoutput) ;
	ProtocolTrigger::SerializedMessage Serialize(const ProtocolTrigger::Message& msg, const ProtocolTrigger::TagSelection& tagwl) const ;
};

bool Serializer::Parse(LocalUser* user, const std::string& line, ProtocolTrigger::ParseOutput& parseoutput)
{
	size_t start = line.find_first_not_of(" ");
	
	if (start == std::string::npos)
	{
		return false;
	}
	
	size_t maxline = INPUT_LIMIT - start - 2;

	if (line[start] == '@')
	{
		maxline += MAX_CLIENT_MESSAGE_TAG_LENGTH + 1;
	}

	engine::token_node_stream tokens(line, start, maxline);

	std::string token;
	tokens.get_middle(token);

	if (token[0] == '@')
	{
		if (token.length() > MAX_CLIENT_MESSAGE_TAG_LENGTH)
		{
			user->SendProtocol(ERR_EXCEED_BUFFER, "Input line was too long");
			return false;
		}
		
		size_t maxbrldline = token.length() + INPUT_LIMIT - 1;
		
		if (tokens.get_message().length() > maxbrldline)
		{
			tokens.get_message().erase(maxbrldline);
		}
		
		std::string tagval;
		engine::node_stream ss(token.substr(1), ';');
		
		while (ss.items_extract(token))
		{
			const std::string::size_type p = token.find('=');
		
			if (p != std::string::npos)
			{
				tagval.assign(token, p+1, std::string::npos);
				token.erase(p);
			}
			else
			{
				tagval.clear();
			}

			HandleTag(user, token, tagval, parseoutput.tags);
		}
		
		if (!tokens.get_middle(token))
		{
			return false;
		}
	}

	if (token[0] == ':')
	{
		if (!tokens.get_middle(token))
		{
			return false;
		}
	}

	parseoutput.cmd.assign(token);
	
	while (tokens.get_trailing(token))
	{
		parseoutput.params.push_back(token);
	}

	return true;
}

namespace
{
	void CheckTagLength(std::string& line, size_t prevsize, size_t& length, size_t maxlength)
	{
		const std::string::size_type diffsize = line.size() - prevsize;
	
		if (length + diffsize > maxlength)
		{
			line.erase(prevsize);
		}
		else
		{
			length += diffsize;
		}
	}
}

void Serializer::SerializeTags(const ProtocolTrigger::TagMap& tags, const ProtocolTrigger::TagSelection& tagwl, std::string& line)
{
	size_t client_tag_length = 0;
	size_t server_tag_length = 0;
	
	for (ProtocolTrigger::TagMap::const_iterator i = tags.begin(); i != tags.end(); ++i)
	{
		if (!tagwl.IsSelected(tags, i))
		{
			continue;
		}

		const std::string::size_type prevsize = line.size();
		line.push_back(prevsize ? ';' : '@');
		line.append(i->first);
		const std::string& val = i->second.value;
		
		if (!val.empty())
		{
			line.push_back('=');
			line.append(val);
		}
		
		if (i->first[0] == '+')
		{
			CheckTagLength(line, prevsize, client_tag_length, MAX_CLIENT_MESSAGE_TAG_LENGTH);
		}
		else
		{
			CheckTagLength(line, prevsize, server_tag_length, MAX_SERVER_MESSAGE_TAG_LENGTH);
		}
	}

	if (!line.empty())
	{
		line.push_back(' ');
	}
	
}

ProtocolTrigger::SerializedMessage Serializer::Serialize(const ProtocolTrigger::Message& msg, const ProtocolTrigger::TagSelection& tagwl) const
{
	std::string line;
	SerializeTags(msg.GetTags(), tagwl, line);
	const std::string::size_type brldmsg_begin = line.size();

	if (msg.GetSource())
	{
		line.push_back(':');
		line.append(*msg.GetSource());
		line.push_back(' ');
	}
	
	line.append(msg.GetCommand());

	const ProtocolTrigger::Message::ParamList& params = msg.GetParams();

	if (!params.empty())
	{
		for (ProtocolTrigger::Message::ParamList::const_iterator i = params.begin(); i != params.end()-1; ++i)
		{
			const std::string& param = *i;
			line.push_back(' ');
			line.append(param);
		}

		line.append(" :", 2).append(params.back());
	}

	if (line.length() - brldmsg_begin > INPUT_LIMIT)
	{
		line.erase(brldmsg_begin + INPUT_LIMIT);
	}

	line.append("\r\n", 2);
	return line;
}

class ModuleCoreSerializer : public Module
{
  private:
  
	Serializer serializer;

 public:
 
	ModuleCoreSerializer() : serializer(this)
	{
	
	}

	void OnCleanup(extension_item::ExpandableType type, Expandable* item) 
	{
		if (type != extension_item::EXT_USER)
		{
			return;
		}

		LocalUser* const user = IS_LOCAL(static_cast<User*>(item));
	
		if ((user) && (user->serializer == &serializer))
		{
			Kernel->Clients->Disconnect(user, "Protocol serializer module unloading");
		}
	}

	void OnInstanceInit(LocalUser* user) 
	{
		if (!user->serializer)
		{
			user->serializer = &serializer;
		}
	}

	Version GetDescription() 
	{
		return Version("Provides a Serializer to the client.", VF_CORE|VF_BERYLDB);
	}
};

MODULE_LOAD(ModuleCoreSerializer)
