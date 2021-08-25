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

class CommandCheck : public Command
{
 public:

        CommandCheck(Module* parent) : Command(parent, "CHECK", 1, 1)
        {
                flags   = 'r';
                syntax  = "<instance>";
        }

        COMMAND_RESULT Handle(User* user, const Params& parameters)
        {
                 User* const target = Kernel->Clients->FindInstance(parameters[0]);        
                 
                 if (!target)
                 {
                      user->SendProtocol(ERR_INPUT, NOT_FOUND);
                      return FAILED;
                 }
                 
                 Dispatcher::JustAPI(user, BRLD_START_LIST);

                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", "Login", target->login.c_str()), Daemon::Format("%s %s", "login", target->login.c_str()));
                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", "IP", target->GetReadableIP().c_str()), Daemon::Format("%s %s", "ip", target->GetReadableIP().c_str()));

                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", "Select", target->GetSelect().c_str()), Daemon::Format("%s %s", "select", target->GetSelect().c_str()));
                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", "Database", target->GetDatabase()->GetName().c_str()), Daemon::Format("%s %s", "database", target->GetDatabase()->GetName().c_str()));
                 Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", "Connected", convto_string(target->GetConString()).c_str()), Daemon::Format("%s %s", "connected", convto_string(target->GetConString()).c_str()));
                 
                 Dispatcher::JustAPI(user, BRLD_END_LIST);
                 return SUCCESS;
        }
};

class CoreModCheck : public Module
{
 private:

        CommandCheck cmd;

 public:

        CoreModCheck() : cmd(this)
        {

        }

        Version GetDescription() 
        {
                return Version("Provides the check command.", VF_CORE|VF_BERYLDB);
        }
};

MODULE_LOAD(CoreModCheck)
