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
#include "core_admin.h"


CommandFinger::CommandFinger(Module* parent) : Command(parent, "FINGER", 0)
{
        requires = 'e';
}

COMMAND_RESULT CommandFinger::Handle(User* user, const Params& parameters)
{
        const LoginHash& users = Kernel->Clients->GetLogins();

        user->SendProtocol(BRLD_FINGER_BEGIN, "Begin of FINGER list.");

        for (LoginHash::const_iterator i = users.begin(); i != users.end(); ++i)
        {
            	User* const login = i->second;
            	
            	if (login->registered != REG_OK)
            	{
            	        continue;
            	}
            	
            	user->SendProtocol(BRLD_FINGER_LIST, login->instance, Daemon::Format("%s (%s), %s (%s)| Logged: %s", login->instance.c_str(), login->GetReadableIP().c_str(), login->login.c_str(), login->agent.c_str(), Daemon::HumanEpochTime(login->connected).c_str()));
        }

    	user->SendProtocol(BRLD_FINGER_END, Daemon::Format("End of FINGER list (%li)", users.size()).c_str());
	return SUCCESS;
}


CommandPause::CommandPause(Module* parent) : Command(parent, "PAUSE", 1, 1)
{
        requires = 'm';
        syntax = "<instance>";
}

COMMAND_RESULT CommandPause::Handle(User* user, const Params& parameters)
{
        User* target = Kernel->Clients->FindInstance(parameters[0]);

        if (!target)
        {
                user->SendProtocol(Protocols::NoInstance(parameters[0]));
                return FAILED;
        }
        
        target->Paused = true;
        
        user->SendProtocol(1, target->instance, PROCESS_OK);
        return SUCCESS;
}

CommandResume::CommandResume(Module* parent) : Command(parent, "RESUME", 1, 1)
{
        requires = 'm';
        syntax = "<instance>";
}

COMMAND_RESULT CommandResume::Handle(User* user, const Params& parameters)
{
        User* target = Kernel->Clients->FindInstance(parameters[0]);

        if (!target)
        {
                user->SendProtocol(Protocols::NoInstance(parameters[0]));
                return FAILED;
        }

        target->Paused = false;

        user->SendProtocol(1, target->instance, PROCESS_OK);
        return SUCCESS;
}

CommandIdle::CommandIdle(Module* parent) : Command(parent, "IDLE", 1, 1)
{
        requires = 'm';
        syntax = "<instance>";
}

COMMAND_RESULT CommandIdle::Handle(User* user, const Params& parameters)
{
        User* target = Kernel->Clients->FindInstance(parameters[0]);

        if (!target)
        {
                user->SendProtocol(Protocols::NoInstance(parameters[0]));
                return FAILED;
        }	

        LocalUser* luser = IS_LOCAL(target);
        
        if (luser)
        {
                time_t idle = Kernel->Now() - luser->touchbase;
                user->SendProtocol(1, target->instance, idle);
                return SUCCESS;
        }
        
        return FAILED;
}
