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

        user->SendProtocol(BRLD_LOGINS_BEGIN, "Begin of LOGINS list.");

        for (UserVector::const_iterator i = logins.begin(); i != logins.end(); ++i)
        {
            	User* const login = *i;
            	user->SendProtocol(BRLD_LOGINS_LIST, Daemon::Format("%s (%s), %s (%s)", login->instance.c_str(), login->GetReadableIP().c_str(), login->agent.c_str(), Daemon::HumanEpochTime(login->connected).c_str()));
        }

    	user->SendProtocol(BRLD_LOGINS_END, Daemon::Format("End of LOGINS list (%li)", logins.size()).c_str());
	return SUCCESS;

}