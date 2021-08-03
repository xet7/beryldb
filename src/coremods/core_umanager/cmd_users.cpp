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
#include "core_umanager.h"

CommandAddUser::CommandAddUser(Module* parent) : Command(parent, "ADDUSER", 2, 2)
{
        requires = 'r';
        syntax = "<login> <password>";
}

COMMAND_RESULT CommandAddUser::Handle(User* user, const Params& parameters)
{
        const std::string& newlogin 	= 	parameters[0];
        const std::string& pass 	= 	parameters[1];
        
        if (newlogin.length() < 3 || newlogin.length() > 15)
        {
                user->SendProtocol(ERR_INPUT, INVALID_FORMAT);
                return FAILED;
        }

        if (!Kernel->Engine->ValidLogin(newlogin))
        {
                user->SendProtocol(ERR_INPUT, INVALID_FORMAT);
                return FAILED;
        }
        
        if (pass.length() < 3 || pass.length() > 30)
        {
                user->SendProtocol(ERR_INPUT, INVALID_FORMAT);
                return FAILED; 
        }
        
        const std::string& exists = UserHelper::Find(newlogin, "created");
        
        if (!exists.empty())
        {
                user->SendProtocol(ERR_INPUT, ENTRY_DEFINED);
                return FAILED;
        }
        
        /* We can now insert the user. */
        
        if (UserHelper::Add(newlogin, pass))
        {
                user->SendProtocol(BRLD_OK, PROCESS_OK);
        }

        return SUCCESS;
}

CommandDelUser::CommandDelUser(Module* parent) : Command(parent, "DELUSER", 1, 1)
{
        requires = 'r';
        syntax = "<login>";
}

COMMAND_RESULT CommandDelUser::Handle(User* user, const Params& parameters)
{
        const std::string& newlogin = parameters[0];
        
        if (newlogin.length() < 3 || newlogin.length() > 15)
        {
                user->SendProtocol(ERR_INPUT, USER_MIMMAX_LENGTH);
                return FAILED;
        }
        
        if (newlogin == "root")
        {
                user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
                return FAILED;
        }
        
        if (!Kernel->Engine->ValidLogin(newlogin))
        {
                user->SendProtocol(ERR_INPUT, INVALID_UNAME);
                return FAILED;
        }

        std::string exists = UserHelper::Find(newlogin, "created");
        
        if (exists.empty())
        {
                user->SendProtocol(ERR_INPUT, PROCESS_FALSE);
                return FAILED;
        }
        
        if (UserHelper::Remove(newlogin))
        {
                user->SendProtocol(BRLD_OK, PROCESS_OK);
                ClientManager::DisconnectAll(newlogin, PROCESS_OK);
        }

        return SUCCESS;
}

CommandListUsers::CommandListUsers(Module* parent) : Command(parent, "LISTUSERS", 0)
{
         requires = 'r';
}

COMMAND_RESULT CommandListUsers::Handle(User* user, const Params& parameters)
{
        const Args& users = STHelper::HKeys("userlist");
        
        Dispatcher::JustAPI(user, BRLD_START_LIST);

        Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s | %-10s", "User", "Created"));
        Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s | %-10s", Dispatcher::Repeat("―", 30).c_str(), Dispatcher::Repeat("―", 10).c_str()));

        for (Args::const_iterator i = users.begin(); i != users.end(); i++)
        {
                const std::string item = *i;
                std::string created = UserHelper::Find(item, "created");

                if (created.empty())
                {
                      continue;
                }

                Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", item.c_str(), Daemon::HumanEpochTime(convto_num<time_t>(created)).c_str()), Daemon::Format("%s %s", item.c_str(), created.c_str()));
        }        
        
        Dispatcher::JustAPI(user, BRLD_USER_LIST_END);
        return SUCCESS;
}

CommandListAdmins::CommandListAdmins(Module* parent) : Command(parent, "LISTADMINS", 0)
{
        requires = 'r';
}

COMMAND_RESULT CommandListAdmins::Handle(User* user, const Params& parameters)
{
        const Args& users = STHelper::HKeys("userlist");

        Dispatcher::JustAPI(user, BRLD_START_LIST);

        Dispatcher::JustEmerald(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", "Admin", "Flag"));
        Dispatcher::JustEmerald(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", Dispatcher::Repeat("―", 30).c_str(), Dispatcher::Repeat("―", 10).c_str()));

        for (Args::const_iterator i = users.begin(); i != users.end(); i++)
        {
                const std::string item = *i;
                std::string flags = UserHelper::CheckFlags(item);
                
                if (flags.empty())
                {
                      continue;
                }
                
                Dispatcher::ListDepend(user, BRLD_USER_ITEM, Daemon::Format("%-30s | %-10s", item.c_str(), flags.c_str()), Daemon::Format("%s %s", item.c_str(), flags.c_str()));
        }

        Dispatcher::JustAPI(user, BRLD_END_LIST);
        return SUCCESS;
}


CommandPasswd::CommandPasswd(Module* parent) : Command(parent, "PASSWD", 1, 2)
{
        syntax = "<password>";
}

COMMAND_RESULT CommandPasswd::Handle(User* user, const Params& parameters)
{
        if (parameters.size() < 2)
        {
                const std::string& pass = parameters[0];

                if (UserHelper::ChangePass(user->login, pass))
                {
                        user->SendProtocol(BRLD_OK, PROCESS_OK);
                }
                else
                {
                        user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
                        return FAILED;
                }

                return SUCCESS;
        }
        
        /* Check if user has root access. */
        
        if (!user->IsAdmin())
        {
                user->SendProtocol(ERR_INPUT, ACCESS_DENIED);
                return FAILED;        
        }
                
        const std::string& newlogin = parameters[0];
        const std::string& pass = parameters[1];

        if (newlogin.length() < 3 || newlogin.length() > 15)
        {
                user->SendProtocol(ERR_INPUT, USER_MIMMAX_LENGTH);
                return FAILED;
        }

        if (!Kernel->Engine->ValidLogin(newlogin))
        {
                user->SendProtocol(ERR_INPUT, INVALID_UNAME);
                return FAILED;
        }
        
        if (pass.length() < 3 || pass.length() > 30)
        {
                user->SendProtocol(ERR_INPUT, PASS_MIMMAX_LENGTH);
                return FAILED; 
        }
        
        std::string exists = UserHelper::Find(newlogin, "created");
        
        if (exists.empty())
        {
                user->SendProtocol(ERR_INPUT, PROCESS_FALSE);
                return FAILED;
        }
        
        if (UserHelper::ChangePass(newlogin, pass))
        {
                user->SendProtocol(BRLD_OK, PROCESS_OK);
        }
        else
        {
                user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
                return FAILED;
        }
        
        return SUCCESS;
}

CommandResetCache::CommandResetCache(Module* parent) : Command(parent, "RESETCACHE", 0)
{
         requires = 'm';
}

COMMAND_RESULT CommandResetCache::Handle(User* user, const Params& parameters)
{
        Kernel->Logins->Reset();
        
        user->SendProtocol(BRLD_OK, PROCESS_OK);
        return SUCCESS;
}

CommandInCache::CommandInCache(Module* parent) : Command(parent, "INCACHE", 1, 1)
{
         requires = 'm';
         syntax = "<login>";
}

COMMAND_RESULT CommandInCache::Handle(User* user, const Params& parameters)
{
        const std::string& login = parameters[0];

        user->SendProtocol(BRLD_OK, convto_string(Kernel->Logins->InCache(login)));
        return SUCCESS;
}
