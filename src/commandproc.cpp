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

bool Daemon::PassCompare(Expandable* ex, const std::string& data, const std::string& input, const std::string& hashtype)
{
	ModuleResult res;
	UNTIL_RESULT(OnPassCompare, res, (ex, data, input, hashtype));

	if (res == MOD_RES_OK)
	{
		return true;
	}

	if (res == MOD_RES_STOP)
	{
		return false;
	}

	if (!hashtype.empty() && !stdhelpers::string::equalsci(hashtype, "plaintext"))
	{
		return false;
	}

	return Daemon::TimingSafeCompare(data, input);
}

bool CommandHandler::HasLoop(User* user, Command* handler, const CommandModel::Params& parameters, unsigned int splithere, int extra, bool usemax)
{
	if (splithere >= parameters.size())
	{
		return false;
	}

	if (parameters[splithere].find(',') == std::string::npos)
	{
		return false;
	}

	brld::flat_set<std::string, engine::insensitive_swo> dupes;
	
	bool check_dupes = (extra < 0);

	engine::comma_node_stream items1(parameters[splithere]);
	engine::comma_node_stream items2(extra >= 0 ? parameters[extra] : "", true);
	std::string item;
	unsigned int max = 0;
	LocalUser* localuser = IS_LOCAL(user);
	
	CommandModel::Params splitparams(parameters);
	
	while (items1.items_extract(item) && (!usemax || max++ < MAX_DEST))
	{
		if ((!check_dupes) || (dupes.insert(item).second))
		{
			splitparams[splithere] = item;

			if (extra >= 0)
			{
				items2.items_extract(item);
				splitparams[extra] = item;
			}

			COMMAND_RESULT result = handler->Handle(user, splitparams);

			if (localuser)
			{
				item.clear();
				NOTIFY_MODS(OnPostCommand, (handler, splitparams, localuser, result, true));
			}
		}
	}

	return true;
}

Command* CommandHandler::GetBase(const std::string &commandname)
{
	CommandMap::iterator n = CommandList.find(commandname);
	
	if (n != CommandList.end())
	{
		return n->second;
	}

	return NULL;
}

COMMAND_RESULT CommandHandler::CallHandler(const std::string& commandname, const CommandModel::Params& parameters, User* user, Command** cmd)
{
	CommandMap::iterator n = CommandList.find(commandname);

	if (n != CommandList.end())
	{
		if ((!parameters.empty()) && (parameters.back().empty()) && (!n->second->last_empty_ok))
		{
			return INVALID;
		}

		if (parameters.size() >= n->second->min_params)
		{
			bool cready = false;

			if (IS_LOCAL(user) && n->second->requires)
			{
				if (user->CanPerform(n->second->requires))
				{
					cready = true;
				}
			}
			else
			{
				
				cready = true;
			}

			if (cready)
			{
				if (cmd)
				{
					*cmd = n->second;
				}

				ProtocolTrigger::TagMap tags;
				return n->second->Handle(user, CommandModel::Params(parameters, tags));
			}
		}
	}

	return INVALID;
}

void CommandHandler::run_command(LocalUser* user, std::string& command, CommandModel::Params& command_p)
{
	if (user->Paused)
	{
		/* PONG can still be sent if user is paused. */
		
		if (command != "PONG")
		{
			return;
		}
	}
	
	if (user->IsLocked())
	{
  	    this->Queue->Add(user, command, command_p);
  	    return;
	}
	
	Command* handler = GetBase(command);

	if (!handler)
	{
		ModuleResult MOD_RESULT;
		UNTIL_RESULT(OnPreCommand, MOD_RESULT, (command, command_p, user, false));
		
		if (MOD_RESULT == MOD_RES_STOP)
		{
			NOTIFY_MODS(OnCommandBlocked, (command, command_p, user));
			return;
		}

		
		handler = GetBase(command);
	
		if (!handler)
		{
		        /* Reports not found command to monitoring clients. */
		        
		        Kernel->Monitor->Push(user->instance, command, MONITOR_DEBUG, command_p);
		
			if (user->registered == REG_OK)
			{
				user->SendProtocol(ERR_CMD_NOFND, command, CMD_NOT_FOUND.c_str());
			}

			Kernel->Stats.Unknown++;
			NOTIFY_MODS(OnCommandBlocked, (command, command_p, user));
			return;
		}
	}

	if (handler->max_params && command_p.size() > handler->max_params)
	{
		const CommandModel::Params::iterator lastbase = command_p.begin() + (handler->max_params - 1);
		const CommandModel::Params::iterator firstexcess = lastbase + 1;

		for (CommandModel::Params::const_iterator i = firstexcess; i != command_p.end(); ++i)
		{
			lastbase->push_back(' ');
			lastbase->append(*i);
		}
		
		command_p.erase(firstexcess, command_p.end());
	}

	ModuleResult MOD_RESULT;
	UNTIL_RESULT(OnPreCommand, MOD_RESULT, (command, command_p, user, false));
	
	Kernel->Monitor->Push(user->instance, command, MONITOR_DEFAULT, command_p);

	if (MOD_RESULT == MOD_RES_STOP)
	{
		NOTIFY_MODS(OnCommandBlocked, (command, command_p, user));
		return;
	}
	
	/* Touch base is updated, as long as command is not PONG or PUBLISH */
	
	if (command != "PONG" && command != "PUBLISH")
	{
		user->touchbase = Kernel->Now();
	}
	
	user->next_ping_check = Kernel->Now() + PING_INTVL;

	/* Verifies whether command requires special flags. */
	
	if (handler->requires)
	{
		if (!user->CanPerform(handler->requires))
		{
			user->SendProtocol(ERR_NO_FLAGS, ACCESS_DENIED);
			NOTIFY_MODS(OnCommandBlocked, (command, command_p, user));
			return;
		}
	}

	if ((!command_p.empty()) && (command_p.back().empty()) && (!handler->last_empty_ok))
	{
		command_p.pop_back();
	}

	/* Verifies whether user provided required parameters. */
	
	if (command_p.size() < handler->min_params)
	{
		handler->MissingParameters(user, command_p);
		NOTIFY_MODS(OnCommandBlocked, (command, command_p, user));
		return;
	}

	if ((user->registered != REG_OK) && (!handler->pre_reg_ok))
	{
		handler->NotLogged(user, command_p);
		NOTIFY_MODS(OnCommandBlocked, (command, command_p, user));
	}
	else
	{
		UNTIL_RESULT(OnPreCommand, MOD_RESULT, (command, command_p, user, true));

		if (MOD_RESULT == MOD_RES_STOP)
		{
			NOTIFY_MODS(OnCommandBlocked, (command, command_p, user));
			return;
		}

		COMMAND_RESULT result = handler->Handle(user, command_p);
		NOTIFY_MODS(OnPostCommand, (handler, command_p, user, result, false));
	}
}

void CommandHandler::Remove(Command* x)
{
	CommandMap::iterator n = CommandList.find(x->name);

	if (n != CommandList.end() && n->second == x)
	{
		CommandList.erase(n);
	}
}

void CommandHandler::ProcessBuffer(LocalUser* user, const std::string& buffer)
{
	ProtocolTrigger::ParseOutput parseoutput;

	if (!user->serializer->Parse(user, buffer, parseoutput))
	{
		return;
	}

	std::string& command = parseoutput.cmd;
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	CommandModel::Params parameters(parseoutput.params, parseoutput.tags);
	run_command(user, command, parameters);
}

bool CommandHandler::add_command(Command *cmd)
{
	if (this->CommandList.find(cmd->name) == this->CommandList.end())
	{
		this->CommandList[cmd->name] = cmd;
		return true;
	}
	
	return false;
}

CommandHandler::CommandHandler()
{

}

std::string CommandHandler::TranslateUIDs(const std::vector<InterpretationType>& to, const CommandModel::Params& source, bool prefix_final, CommandModel* custom_translator)
{
	std::vector<InterpretationType>::const_iterator types = to.begin();
	std::string dest;

	for (unsigned int i = 0; i < source.size(); i++)
	{
		InterpretationType t = TR_TEXT;

		if (types != to.end())
		{
			t = *types;
			types++;
		}

		bool last = (i == (source.size() - 1));

		if (prefix_final && last)
		{
			dest.push_back(':');
		}

		TranslateSingleParam(t, source[i], dest, custom_translator, i);

		if (!last)
		{
			dest.push_back(' ');
		}
	}

	return dest;
}

void CommandHandler::TranslateSingleParam(InterpretationType to, const std::string& item, std::string& dest, CommandModel* custom_translator, unsigned int paramnumber)
{
	switch (to)
	{
		case TR_INSTANCE:
		{
			User* user = Kernel->Clients->FindInstance(item);
			
			if (user)
			{
				dest.append(user->uuid);
			}
			else
			{
				dest.append(item);
			}
			
			break;
		}
		
		case TR_CUSTOM:
		{
			if (custom_translator)
			{
				std::string translated = item;
				custom_translator->EncodeParameter(translated, paramnumber);
				dest.append(translated);
			}
			
		    break;
		}
		
		default:
			
			dest.append(item);
		break;
	}
}

CommandQueue::CommandQueue()
{

}

void CommandQueue::Add(LocalUser* user, std::string& command, CommandModel::Params& command_p)
{
	if (!user || user->IsQuitting())
        {
             return;
        }

        PendingCMD adding(user, command_p, command);
        this->PendingList.push_back(adding);
}

void CommandQueue::Flush()
{
       if (!this->PendingList.size())
       {
            return;
       }
       
       PendingCMD event = this->PendingList.front();
       
       LocalUser* user = event.user;
       
       if (!user || user->IsQuitting())
       {
		this->PendingList.pop_front();
		return;
       }
       
       if (user->IsLocked())
       {
	      this->PendingList.push_back(event);
	      this->PendingList.pop_front();
       	      return;
       }

       this->PendingList.pop_front();
       
       Kernel->Commander.run_command(user, event.command, event.command_p);
}
