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

#include <sstream>

#include "beryl.h"
#include "core_umanager.h"
#include "managers/user.h"

CommandFlags::CommandFlags(Module* parent) : Command(parent, "FLAGS", 1, 1)
{
        requires = 'r';
        syntax = "<login> <flags>";
}

COMMAND_RESULT CommandFlags::Handle(User* user, const Params& parameters)
{
        const std::string& newlogin = parameters[0];

        if (newlogin.length() < 3 || newlogin.length() > 15)
        {
                user->SendProtocol(ERR_INVALID_PARAM, newlogin, USER_MIMMAX_LENGTH);
                return FAILED;
        }
        
        std::string exists = UserHelper::FindAdmin(newlogin);

        /* Checks if user has flags. */
        
        if (!exists.empty())
        {
                user->SendProtocol(BRLD_LOGIN_FLAGS, newlogin, exists);
                return SUCCESS;
        }
        else 
        {
                user->SendProtocol(ERR_LOGIN_NO_FLAGS, "Flags not found.");
        }
        
        return FAILED;
        
       
}

CommandDelFlags::CommandDelFlags(Module* parent) : Command(parent, "DELFLAGS", 2, 2)
{
        requires = 'r';
        syntax = "<login> <flags>";
}

COMMAND_RESULT CommandDelFlags::Handle(User* user, const Params& parameters)
{
        const std::string& newlogin = parameters[0];
        const std::string& flag = parameters[1];
        
        if (flag.empty())
        {
            user->SendProtocol(ERR_LOGIN_NO_FLAGS, "Incorrect flag format.");
            return FAILED;
        }

        if (flag.length() > 1)
        {
                user->SendProtocol(BRLD_ADD_FLAGS, Daemon::Format("Invalid flag: %s", flag.c_str()));
                return FAILED;        
        }

        if (!UserHelper::Exists(newlogin))
        {
                user->SendProtocol(BRLD_ADD_FLAGS, Daemon::Format("User does not exists: %s", newlogin.c_str()));
                return FAILED;             
        }

        if (newlogin == "root")
        {
                user->SendProtocol(ERR_PROTECTED_LOGIN, "You cannot modify root's flags.");
                return FAILED;
        }
        
        bool exists = UserHelper::AdminExists(newlogin);
        
        if (!exists)
        {
            user->SendProtocol(ERR_NO_FLAGS, "Admin does not exists.");
            return FAILED;
        }
        
        for (auto it = flag.cbegin() ; it != flag.cend(); ++it) 
        {
                if (*it != 'e' && *it != 'r' && *it != 'm')
                {
                        user->SendProtocol(BRLD_ADD_FLAGS, Daemon::Format("Invalid flag: %c", *it));
                        return FAILED;
                }
        }
        
        std::string adding = UserHelper::FindAdmin(newlogin);
        std::string creating;
        
        for (auto it = adding.cbegin() ; it != adding.cend(); ++it) 
        {
                if (*it == flag[0])
                {
                    continue;
                }
                
                creating.push_back(*it);
        }

        Kernel->Logins->Sessions->AttachExternal(newlogin, creating);
        
        UserHelper::SetAdmin(newlogin, creating);
        
        if (creating.empty())
        {
            /* We delete the user from the TABLE_ADMIN */
            
            UserHelper::RemoveAdmin(newlogin);
            user->SendProtocol(BRLD_ADD_FLAGS, Daemon::Format("%s is no longer with any flags.", newlogin.c_str()));
        }
        else
        {
            user->SendProtocol(BRLD_ADD_FLAGS, Daemon::Format("Delete. Flags for %s: %s", newlogin.c_str(), creating.c_str()));
        }
        
        /* We notify all users connected about the change */

        Kernel->Logins->Sessions->NotifyFlags(newlogin, creating);
        
        
        return SUCCESS;
}

CommandSetFlags::CommandSetFlags(Module* parent) : Command(parent, "SETFLAGS", 2, 2)
{
        requires = 'r';
        syntax = "<login> <flags>";
}

COMMAND_RESULT CommandSetFlags::Handle(User* user, const Params& parameters)
{
        const std::string& newlogin = parameters[0];
        const std::string& flag = parameters[1];
        
        if (newlogin == "root")
        {
                user->SendProtocol(ERR_PROTECTED_LOGIN, newlogin, "You cannot modify root's flags.");
                return FAILED;
        }
        
        if (flag.length() > 1)
        {
                user->SendProtocol(BRLD_ADD_FLAGS, Daemon::Format("Invalid flag: %s", flag.c_str()));
                return FAILED;        
        }
        
        for (auto it = flag.cbegin() ; it != flag.cend(); ++it) 
        {
                if (*it != 'e' && *it != 'r' && *it != 'm')
                {
                        user->SendProtocol(BRLD_ADD_FLAGS, Daemon::Format("Invalid flag: %s", flag.c_str()));
                        return FAILED;
                }
        }

        if (!UserHelper::Exists(newlogin))
        {
                user->SendProtocol(BRLD_ADD_FLAGS, newlogin, Daemon::Format("User does not exists: %s", newlogin.c_str()));
                return FAILED;             
        }
  
        std::string exists = UserHelper::FindAdmin(newlogin);
        std::string adding;
        
        for (auto it = exists.cbegin() ; it != exists.cend(); ++it) 
        {
                if (*it == flag[0])
                {
                        user->SendProtocol(BRLD_ADD_FLAGS, newlogin, Daemon::Format("User %s already has flag: %s", newlogin.c_str(), flag.c_str()));
                        return FAILED;
                }
                
                adding.push_back(*it);
        }
        
        adding.push_back(flag[0]);
        
        UserHelper::SetAdmin(newlogin, adding);
        Kernel->Logins->Sessions->AttachExternal(newlogin, adding);

        /* We notify all users connected about the change */
        
        Kernel->Logins->Sessions->NotifyFlags(newlogin, adding);
        
        user->SendProtocol(BRLD_ADD_FLAGS, Daemon::Format("Assigned flags for %s: %s", newlogin.c_str(), adding.c_str()));
        return SUCCESS;
}

