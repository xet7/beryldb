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
#include "managers/user.h"
#include "brldb/database.h"
#include "managers/maps.h"

CommandAddUser::CommandAddUser(Module* parent) : Command(parent, "ADDUSER", 2, 2)
{
        requires = 'r';
        syntax = "<login> <password>";
}

COMMAND_RESULT CommandAddUser::Handle(User* user, const Params& parameters)
{
        const std::string& newlogin = parameters[0];
        const std::string& pass = parameters[1];
        
        if (newlogin.length() < 3 || newlogin.length() > 15)
        {
                user->SendProtocol(ERR_INVALID_LOGIN, newlogin, USER_MIMMAX_LENGTH);
                return FAILED;
        }

        if (!Kernel->Engine->ValidLogin(newlogin))
        {
                user->SendProtocol(ERR_INVALID_LOGIN, newlogin, INVALID_UNAME);
                return FAILED;
        }
        
        if (pass.length() < 3 || pass.length() > 30)
        {
                user->SendProtocol(ERR_INVALID_PASS, PASS_MIMMAX_LENGTH);
                return FAILED; 
        }
        
        const std::string exists = UserHelper::Find(newlogin, "created");
        
        if (!exists.empty())
        {
                user->SendProtocol(ERR_LOGIN_EXISTS, exists, Daemon::Format("Login %s is in the system already.", newlogin.c_str()).c_str());
                return FAILED;
        }
        
        /* We can now insert the user. */
        
        if (UserHelper::Add(newlogin, pass))
        {
                user->SendProtocol(BRLD_USER_ADD, newlogin, "User added.");
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
                user->SendProtocol(ERR_INVALID_LOGIN, newlogin, USER_MIMMAX_LENGTH);
                return FAILED;
        }
        
        if (newlogin == "root")
        {
                user->SendProtocol(ERR_PROTECTED_LOGIN, "You cannot remove root.");
                return FAILED;
        }
        
        if (!Kernel->Engine->ValidLogin(newlogin))
        {
                user->SendProtocol(ERR_LOGIN_EXISTS, newlogin, INVALID_UNAME);
                return FAILED;
        }

        std::string exists = UserHelper::Find(newlogin, "created");
        
        if (exists == "")
        {
                user->SendProtocol(ERR_LOGIN_EXISTS, INVALID_UNAME, newlogin, Daemon::Format("User %s does not exists.", newlogin.c_str()).c_str());
                return FAILED;
        }
        
        if (UserHelper::Remove(newlogin))
        {
                user->SendProtocol(BRLD_LOGIN_DEL, newlogin, Daemon::Format("User %s removed.", newlogin.c_str()).c_str());
                SessionManager::DisconnectAll(newlogin);
        }

        return SUCCESS;
}

CommandListUsers::CommandListUsers(Module* parent) : Command(parent, "LISTUSERS", 0)
{
         requires = 'r';
}

COMMAND_RESULT CommandListUsers::Handle(User* user, const Params& parameters)
{
        unsigned int counter = 0;

        MMapTuple tpl = MapsHelper::SearchHesh(TABLE_USERS, "userlogin");
        DualMMap users = std::get<1>(tpl);
        
        for (DualMMap::iterator i = users.begin(); i != users.end(); i++)
        {
                counter++;
                std::string entry = i->first;
                user->SendProtocol(BRLD_USER_LIST, counter, entry, Daemon::Format("%i: %s", counter, entry.c_str()).c_str());
        }
        
        user->SendProtocol(BRLD_USER_LIST_END, counter, "End of LISTUSERS list.");
        
        return SUCCESS;
}

CommandListAdmins::CommandListAdmins(Module* parent) : Command(parent, "LISTADMINS", 0)
{
        requires = 'r';
}

COMMAND_RESULT CommandListAdmins::Handle(User* user, const Params& parameters)
{
        unsigned int counter = 0;
       
        MMapTuple tpl = MapsHelper::SearchHesh(TABLE_ADMIN, "flags");
        DualMMap users = std::get<1>(tpl);
        
        for (DualMMap::iterator i = users.begin(); i != users.end(); i++)
        {
                counter++;
                std::string entry = i->first;
                user->SendProtocol(BRLD_USER_LIST, counter, entry, Daemon::Format("%i: %s", counter, entry.c_str()).c_str());
        }
        
        user->SendProtocol(BRLD_USER_LIST_END, counter, "End of LISTADMINS list.");
        
        return SUCCESS;
}

