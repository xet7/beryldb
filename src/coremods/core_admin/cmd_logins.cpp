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

CommandLogins::CommandLogins(Module* parent) : Command(parent, "LOGINS", 1, 1)
{
        requires = 'e';
        syntax = "<login>";
}

COMMAND_RESULT CommandLogins::Handle(User* user, const Params& parameters)
{
        const std::string& loginid = parameters[0];
        
        UserVector logins = Kernel->Clients->FindLogin(loginid);

        Dispatcher::JustAPI(user, BRLD_LOGINS_BEGIN);

        for (UserVector::const_iterator i = logins.begin(); i != logins.end(); ++i)
        {
            	User* const login = *i;
            	user->SendProtocol(BRLD_LOGINS_ITEM, Daemon::Format("%s (%s), %s (%s)", login->instance.c_str(), login->GetReadableIP().c_str(), login->agent.c_str(), Daemon::HumanEpochTime(login->connected).c_str()));
        }

        Dispatcher::JustAPI(user, BRLD_LOGINS_END);
	return SUCCESS;
}


CommandFindFlags::CommandFindFlags(Module* parent) : Command(parent, "LISTFLAGS", 0, 1)
{
        requires = 'e';
        syntax = "<flag>";
}

COMMAND_RESULT CommandFindFlags::Handle(User* user, const Params& parameters)
{
        std::string flag;
        
        if (parameters.size())
        {
                flag = parameters[0];
        }
        
        UserVector logins = ClientManager::FindPrivs(flag);

        Dispatcher::JustAPI(user, BRLD_LOGINS_BEGIN);

        for (UserVector::const_iterator i = logins.begin(); i != logins.end(); ++i)
        {
                User* const login = *i;
                user->SendProtocol(BRLD_LOGINS_ITEM, Daemon::Format("%s (%s) %s", login->instance.c_str(), login->login.c_str(), login->session->rawflags.c_str()).c_str());
        }

        Dispatcher::JustAPI(user, BRLD_LOGINS_END);
        
        return SUCCESS;

}
