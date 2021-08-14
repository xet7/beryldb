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

	user->SendRemoteProtocol(BRLD_OK2, Kernel->Config->GetServerName(), Daemon::HumanEpochTime(Kernel->Now()));
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

        user->SendRemoteProtocol(BRLD_OK2, Kernel->Config->GetServerName(), convto_string(Kernel->Now()));
        return SUCCESS;
}

CommandL::CommandL(Module* parent) : Command(parent, "I", 0)
{

}

COMMAND_RESULT CommandL::Handle(User* user, const Params& parameters)
{
        Dispatcher::JustAPI(user, BRLD_START_LIST);

        user->SendProtocol(BRLD_ITEM_LIST, Daemon::Format("%-9s | %s", "Connected", Daemon::HumanEpochTime(user->GetConnected()).c_str()));
        user->SendProtocol(BRLD_ITEM_LIST, Daemon::Format("%-9s | %s", "Created", Daemon::HumanEpochTime(Kernel->Store->GetCreated()).c_str()).c_str());
        
        user->SendProtocol(BRLD_ITEM_LIST, Daemon::Format("%-9s | %s", "Version", Kernel->GetVersion(user->CanPerform('e')).c_str()).c_str());
        user->SendProtocol(BRLD_ITEM_LIST, Daemon::Format("%-9s | %s", "Select", user->select.c_str()));
        user->SendProtocol(BRLD_ITEM_LIST, Daemon::Format("%-9s | %s", "Agent", user->agent.c_str()));

        /* Returns admin flags to requesting user, if any. */

        const std::string& exists = UserHelper::CheckFlags(user->login);

        if (!exists.empty())
        {
                user->SendProtocol(BRLD_ITEM_LIST, Daemon::Format("%-9s | %s", "Flags", exists.c_str()).c_str());
        }
     
        if (user->GetDatabase())
        {
             user->SendProtocol(BRLD_ITEM_LIST, Daemon::Format("%-9s | %s", "Database", user->GetDatabase()->GetName().c_str()));
        }
        
        const std::string& all_groups = user->GetAllGroups();
        
        if (!all_groups.empty())
        {
                user->SendProtocol(BRLD_ITEM_LIST, Daemon::Format("%-9s | %s", "Groups", all_groups.c_str()).c_str());
        }
        
        /* Requesting user login. */

        user->SendProtocol(BRLD_ITEM_LIST, Daemon::Format("%-9s | %s", "Instance", user->instance.c_str()));	
        user->SendProtocol(BRLD_ITEM_LIST, Daemon::Format("%-9s | %s", "Login", user->login.c_str())); 

        Dispatcher::JustAPI(user, BRLD_END_LIST);
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
	
      user->SendProtocol(BRLD_OK, Kernel->GetVersion(user->CanPerform('e')));
      return SUCCESS;
}

CommandAgent::CommandAgent(Module* parent) : Command(parent, "MYAGENT", 0, 0)
{

}

COMMAND_RESULT CommandAgent::Handle(User* user, const Params& parameters)
{
         user->SendProtocol(BRLD_OK, user->agent.c_str());
         return SUCCESS;
}

CommandWhoami::CommandWhoami(Module* parent) : Command(parent, "WHOAMI", 0, 0)
{

}

COMMAND_RESULT CommandWhoami::Handle(User* user, const Params& parameters)
{
         user->SendProtocol(BRLD_OK, user->login.c_str());
         return SUCCESS;
}

CommandFirstOf::CommandFirstOf(Module* parent) : Command(parent, "FIRSTOF", 1, 1)
{
         flags = 'm';
         syntax = "<user>";
}

COMMAND_RESULT CommandFirstOf::Handle(User* user, const Params& parameters)
{
         const User* found = Kernel->Clients->FirstLogin(parameters[0]);
         
         if (found)
         {
                user->SendProtocol(BRLD_OK, found->instance.c_str());
         }
         else
         {
               user->SendProtocol(ERR_INPUT, parameters[0].c_str());
               return FAILED;
         }
         
         return SUCCESS;
}

CommandGS::CommandGS(Module* parent) : Command(parent, "GLS", 0)
{
         group = 'w';
}

COMMAND_RESULT CommandGS::Handle(User* user, const Params& parameters)
{
      DBHelper::GList(user);
      return SUCCESS;
}

CommandLS::CommandLS(Module* parent) : Command(parent, "LS", 0)
{
         group = 'w';
}

COMMAND_RESULT CommandLS::Handle(User* user, const Params& parameters)
{
      DBHelper::List(user);
      return SUCCESS;
}

CommandTotal::CommandTotal(Module* parent) : Command(parent, "TOTAL", 0)
{
         group = 'w';
}

COMMAND_RESULT CommandTotal::Handle(User* user, const Params& parameters)
{
         DBHelper::Total(user);
         return SUCCESS;
}

