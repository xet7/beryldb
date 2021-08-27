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
#include "core_channel.h"

CommandJoin::CommandJoin(Module* parent) : MultiCommand(parent, "JOIN", 1, 1)
{
	syntax = "<channel>[,<channel>]";
}

COMMAND_RESULT CommandJoin::HandleLocal(LocalUser* user, const Params& parameters)
{
	if (parameters.size() > 1)
	{
		if (CommandHandler::HasLoop(user, this, parameters, 0, 1, false))
		{
			return SUCCESS;
		}

		/* We must verify is channel name is valid. */
		
		if (Kernel->Engine->ValidChannel(parameters[0]))
		{
			Channel* chan = Kernel->Channels->Find(parameters[0]);
			
			if (chan)
			{
				if (chan->GetUser(user))
				{
					user->SendProtocol(ERR_INPUT, PROCESS_ALREADY);
					return FAILED;
				}
			}
			
			Channel::JoinUser(false, user, parameters[0], false);
                        std::string login = user->login;
                        
                        if (Kernel->Sets->AsBool(chansync))
                        {
                                Kernel->Clients->Join(user, login, parameters[0]);
                        }

			return SUCCESS;
		}
	}
	else
	{
		if (CommandHandler::HasLoop(user, this, parameters, 0, -1, false))
		{
			return SUCCESS;
		}

		if (Kernel->Engine->ValidChannel(parameters[0]))
		{
		        Channel* chan = Kernel->Channels->Find(parameters[0]);

                        if (chan)
                        {
                                if (chan->GetUser(user))
                                {
                                        user->SendProtocol(ERR_INPUT, PROCESS_ALREADY);
                                        return FAILED;
                                }
                        }

			Channel::JoinUser(false, user, parameters[0]);
			std::string login = user->login;
			
		        /* User will not sync chans */

		        if (Kernel->Sets->AsBool(chansync))
		        {
	                        Kernel->Clients->Join(user, login, parameters[0]);
			}
			
			return SUCCESS;
		}
	}

	user->SendProtocol(ERR_INPUT, INVALID_CHAN);
	return FAILED;
}

CommandPart::CommandPart(Module* parent) : Command(parent, "PART", 1, 1)
{
	syntax = "<channel>[,<channel>]";
}

COMMAND_RESULT CommandPart::Handle(User* user, const Params& parameters)
{
	if (CommandHandler::HasLoop(user, this, parameters, 0))
	{
		return SUCCESS;
	}

	Channel* chan = Kernel->Channels->Find(parameters[0]);

	if (!chan)
	{
		user->SendProtocol(Protocols::NoChannel(parameters[0]));
		return FAILED;
	}

	if (!chan->PartUser(user))
	{
		user->SendProtocol(ERR_INPUT, NOT_SUBSCRIBED);
		return FAILED;
	}
	else
	{
		/* Parts are  being sync */
		
                if (Kernel->Sets->AsBool(chansync))
                {
                	std::string login = user->login;
	                Kernel->Clients->Part(user, login, parameters[0]);
                }
	}

	return SUCCESS;
}

RouteParams CommandPart::GetRouting(User* user, const Params& parameters)
{
	return (IS_LOCAL(user) ? ROUTE_LOCALONLY : ROUTE_BROADCAST);
}

CommandPartAll::CommandPartAll(Module* Creator) : Command(Creator, "PARTALL", 0, 0)
{

}
        
COMMAND_RESULT CommandPartAll::Handle(User* user, const Params& parameters)
{
       if (!user->chans.size())
       {
             user->SendProtocol(ERR_INPUT, NO_CHANS); 
             return FAILED;
       }
               
       user->SendProtocol(BRLD_OK, PROCESS_OK);

       for (User::SubsList::iterator i = user->chans.begin(); i != user->chans.end(); )
       {
             Channel* chan = (*i)->chan;
             ++i;
             chan->PartUser(user);
       }

       return SUCCESS;
}

CommandHop::CommandHop(Module* Creator) : MultiCommand(Creator, "HOP", 1)
{
       syntax = "<channel>";
}

COMMAND_RESULT CommandHop::HandleLocal(LocalUser* user, const Params& parameters)
{
       if (CommandHandler::HasLoop(user, this, parameters, 0))
       {
                return SUCCESS;
       }

       Channel* const channel = Kernel->Channels->Find(parameters[0]);

       if (!channel)
       {
                 user->SendProtocol(Protocols::NoChannel(parameters[0]));
                 return FAILED;
       }

       if (channel->IsSubscribed(user))
       {
                 channel->PartUser(user);
                 Channel::JoinUser(false, user, parameters[0], true);
                 return SUCCESS;
       }
       else
       {
                 user->SendProtocol(ERR_INPUT, NOT_SUBSCRIBED);
       }

       return FAILED;
}

CommandMute::CommandMute(Module* parent) : Command(parent, "MUTE", 1, 1)
{
        flags  = 'e';
        syntax = "<channel>";
}

COMMAND_RESULT CommandMute::Handle(User* user, const Params& parameters)
{
        Channel* const chan = Kernel->Channels->Find(parameters[0]);

        if (!chan)
        {
                user->SendProtocol(Protocols::NoChannel(parameters[0]));
                return FAILED;
        }
        
        if (chan->muted)
        {
                user->SendProtocol(ERR_INPUT, PROCESS_ALREADY);        
                return FAILED;
        }
        
        chan->muted = true;
        user->SendProtocol(BRLD_OK, PROCESS_OK);
        return SUCCESS;
}

CommandUnmute::CommandUnmute(Module* parent) : Command(parent, "UNMUTE", 1, 1)
{
        flags  = 'e';
        syntax = "<channel>";
}

COMMAND_RESULT CommandUnmute::Handle(User* user, const Params& parameters)
{
        Channel* const chan = Kernel->Channels->Find(parameters[0]);

        if (!chan)
        {
                user->SendProtocol(Protocols::NoChannel(parameters[0]));
                return FAILED;
        }

        if (!chan->muted)
        {
               user->SendProtocol(ERR_INPUT, PROCESS_FALSE);        
               return FAILED;
        }
        
        chan->muted = false;
        user->SendProtocol(BRLD_OK, PROCESS_OK);
        return SUCCESS;
}
