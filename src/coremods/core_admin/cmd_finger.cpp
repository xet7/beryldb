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

class AutoResume : public Timer
{
   private:
           
           User* user;
           
   public:
   
           AutoResume(User* usr, unsigned int seconds) : Timer(seconds, false), user(usr)
           {
                   Kernel->Tickers->Add(this);
           }
           
           bool Run(time_t current)
           {
                   if (!this->user || this->user->IsQuitting())
                   {
                           return false;
                   }
                   
                   user->Paused = false;
                   return false;
           }
};

CommandFinger::CommandFinger(Module* parent) : Command(parent, "FINGER", 0)
{
        requires = 'e';
}

COMMAND_RESULT CommandFinger::Handle(User* user, const Params& parameters)
{
        const LoginHash& users = Kernel->Clients->GetLogins();

        Dispatcher::JustAPI(user, BRLD_FINGER_BEGIN);
        Dispatcher::JustEmerald(user, BRLD_FINGER_BEGIN, Daemon::Format("%-30s | %-10s | %-8s | %-10s | %-5s", "Instance", "IP", "Login", "Agent", "Logged"));
        Dispatcher::JustEmerald(user, BRLD_FINGER_BEGIN, Daemon::Format("%-30s | %-10s | %-8s | %-10s | %-5s", Dispatcher::Repeat("―", 30).c_str(), Dispatcher::Repeat("―", 10).c_str(), Dispatcher::Repeat("―", 8).c_str(), Dispatcher::Repeat("―", 10).c_str(), Dispatcher::Repeat("―", 5).c_str()));

        for (LoginHash::const_iterator i = users.begin(); i != users.end(); ++i)
        {
            	User* const login = i->second;
            	
            	if (login->registered != REG_OK)
            	{
            	        continue;
            	}
            	
            	Dispatcher::ListDepend(user, BRLD_FINGER_LIST, Daemon::Format("%-30s | %-10s | %-8s | %-10s | %-5s", login->instance.c_str(), login->GetReadableIP().c_str(), login->login.c_str(), login->agent.c_str(), Daemon::HumanEpochTime(login->GetConnected()).c_str()), Daemon::Format("%s %s %s %s %s", login->instance.c_str(), login->GetReadableIP().c_str(), login->login.c_str(), login->agent.c_str(), Daemon::HumanEpochTime(login->GetConnected()).c_str()));
        }

        Dispatcher::JustAPI(user, BRLD_FINGER_END);
	return SUCCESS;
}


CommandPause::CommandPause(Module* parent) : Command(parent, "PAUSE", 1, 2)
{
        requires = 'm';
        syntax = "<instance> <*seconds>";
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
        
        if (parameters.size() == 2)
        {
             new AutoResume(target, convto_num<unsigned int>(parameters[1]));
        }
        
        user->SendProtocol(BRLD_OK, PROCESS_OK);
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

        user->SendProtocol(BRLD_OK, PROCESS_OK);
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
                user->SendProtocol(BRLD_OK, idle);
                return SUCCESS;
        }
        
        return FAILED;
}

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
            	user->SendProtocol(BRLD_LOGINS_ITEM, Daemon::Format("%s (%s) | %s (%s)", login->instance.c_str(), login->GetReadableIP().c_str(), login->agent.c_str(), Daemon::HumanEpochTime(login->GetConnected()).c_str()));
        }

        Dispatcher::JustAPI(user, BRLD_LOGINS_END);
	return SUCCESS;
}


CommandFindFlags::CommandFindFlags(Module* parent) : Command(parent, "FINDFLAGS", 0, 1)
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
                user->SendProtocol(BRLD_LOGINS_ITEM, Daemon::Format("%-10s (%s) | %-10s", login->instance.c_str(), login->login.c_str(), login->session->GetFlags().c_str()).c_str());
        }

        Dispatcher::JustAPI(user, BRLD_LOGINS_END);
        
        return SUCCESS;

}
