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
#include "core_info.h"
#include "managers/keys.h"
#include "managers/user.h"
#include "managers/databases.h"
#include "brldb/dbmanager.h"

CommandTime::CommandTime(Module* parent) : ServerTargetCommand(parent, "TIME")
{
	syntax = "[<server>]";
}

COMMAND_RESULT CommandTime::Handle(User* user, const Params& parameters)
{
	/* Checks whether this command is routes to us. */
	
	if (parameters.size() > 0 && !engine::equals(parameters[0], Kernel->Config->ServerName))
	{
		return SUCCESS;
	}

	user->SendRemoteProtocol(BRLD_LOCAL_TIME, Kernel->Config->GetServerName(), Daemon::HumanEpochTime(Kernel->Now()));
	return SUCCESS;
}

CommandEpoch::CommandEpoch(Module* parent) : ServerTargetCommand(parent, "EPOCH")
{
        syntax = "[<server>]";
}

COMMAND_RESULT CommandEpoch::Handle(User* user, const Params& parameters)
{
        /* Checks whether this command is routes to us. */
        
        if (parameters.size() > 0 && !engine::equals(parameters[0], Kernel->Config->ServerName))
        {
                return SUCCESS;
        }

        user->SendRemoteProtocol(BRLD_LOCAL_EPOCH, Kernel->Config->GetServerName(), convto_string(Kernel->Now()));
        return SUCCESS;
}

CommandL::CommandL(Module* parent) : Command(parent, "L", 0)
{

}

COMMAND_RESULT CommandL::Handle(User* user, const Params& parameters)
{
        user->SendProtocol(BRLD_START_UNQ_LIST, Daemon::Format("Version: %s", Kernel->GetVersion(user->CanPerform('e')).c_str()).c_str());
        user->SendProtocol(BRLD_VIEW_INFO, Daemon::Format("Current select in use: %s", user->select.c_str()));
        
        if (user->current_db)
        {
             user->SendProtocol(BRLD_DB_NAME, Daemon::Format("Database name: %s", user->current_db->GetName().c_str()));
        }
        
        /* Returns admin flags to requesting user, if any. */
        
        std::string exists = UserHelper::CheckFlags(user->login);
        
        if (!exists.empty())
        {
                user->SendProtocol(BRLD_FLAG_INFO, Daemon::Format("Your flags: %s", exists.c_str()).c_str());
                user->SendProtocol(BRLD_INSTANCE, Daemon::Format("Instance created: %s", Daemon::HumanEpochTime(Kernel->Store->instance).c_str()).c_str());
        }
        
        /* Requesting user login. */

        user->SendProtocol(BRLD_END_UNQ_LIST, Daemon::Format("Your login: %s - Your instance: %s", user->login.c_str(), user->instance.c_str()));	
	return SUCCESS;
}

CommandVersion::CommandVersion(Module* parent) : Command(parent, "VERSION", 0, 0)
{

}

COMMAND_RESULT CommandVersion::Handle(User* user, const Params& parameters)
{
      /* 
       * All connected users can run VERSION command. However, only users 
       * with 'e' or higher flags can obtain a full version of this server. 
       */
	
      user->SendProtocol(BRLD_VERSION, Kernel->GetVersion(user->CanPerform('e')));
      return SUCCESS;
}

CommandAgent::CommandAgent(Module* parent) : Command(parent, "MYAGENT", 0, 0)
{

}

COMMAND_RESULT CommandAgent::Handle(User* user, const Params& parameters)
{
         user->SendProtocol(BRLD_AGENT, user->agent, user->agent.c_str());
         return SUCCESS;
}

CommandWhoami::CommandWhoami(Module* parent) : Command(parent, "WHOAMI", 0, 0)
{

}


COMMAND_RESULT CommandWhoami::Handle(User* user, const Params& parameters)
{
         user->SendProtocol(BRLD_WHOAMI, user->login, user->login.c_str());
         return SUCCESS;
}

CommandFirstOf::CommandFirstOf(Module* parent) : Command(parent, "FIRSTOF", 1, 1)
{
         requires = 'm';
         syntax = "<user>";
}

COMMAND_RESULT CommandFirstOf::Handle(User* user, const Params& parameters)
{
         const User* found = Kernel->Clients->FirstLogin(parameters[0]);
         
         if (found)
         {
                user->SendProtocol(BRLD_FIRSTOF, found->instance.c_str());
         }
         else
         {
               user->SendProtocol(ERR_NOT_FOUND, parameters[0].c_str());
               return FAILED;
         }
         
         return SUCCESS;
}

CommandList::CommandList(Module* parent) : Command(parent, "LIST", 0)
{

}

COMMAND_RESULT CommandList::Handle(User* user, const Params& parameters)
{
      DBHelper::List(user);
      return SUCCESS;
}