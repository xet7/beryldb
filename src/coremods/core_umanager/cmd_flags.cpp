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

CommandListFlags::CommandListFlags(Module* parent) : Command(parent, "LISTFLAGS", 1, 1)
{
        flags = 'r';
        syntax = "<login>";
}

COMMAND_RESULT CommandListFlags::Handle(User* user, const Params& parameters)
{
        const std::string& newlogin = parameters[0];

        if (newlogin.length() < 3 || newlogin.length() > 15)
        {
                user->SendProtocol(ERR_INPUT, USER_MIMMAX_LENGTH);
                return FAILED;
        }

        const std::string& userflags = UserHelper::CheckFlags(newlogin);

        if (!userflags.empty())
        {
                for (auto it = userflags.cbegin() ; it != userflags.cend(); ++it) 
                {
                      if (*it == 'e')
                      {
                           user->SendProtocol(BRLD_LOGIN_FLAGS, "can_execute");
                      }
                      else if (*it == 'r')
                      {
                           user->SendProtocol(BRLD_LOGIN_FLAGS, "can_root");
                      }
                      else if (*it == 'm')
                      {  
                           user->SendProtocol(BRLD_LOGIN_FLAGS, "can_manage");
                      }
                }

                return SUCCESS;
        }
        else 
        {
                user->SendProtocol(ERR_INPUT, ERR_NO_FLAGS);
        }

        return FAILED;
}

CommandGetFlags::CommandGetFlags(Module* parent) : Command(parent, "GETFLAGS", 1, 1)
{
        flags  = 'r';
        syntax = "<login>";
}

COMMAND_RESULT CommandGetFlags::Handle(User* user, const Params& parameters)
{
        const std::string& newlogin = parameters[0];

        if (newlogin.length() < 3 || newlogin.length() > 15)
        {
                user->SendProtocol(ERR_INPUT2, ERR_INVALID_PARAM, PROCESS_FALSE);
                return FAILED;
        }
        
        std::string userflags = UserHelper::CheckFlags(newlogin);

        /* Checks if user has flags. */
        
        if (!userflags.empty())
        {
                user->SendProtocol(BRLD_OK, userflags);
                return SUCCESS;
        }
        else 
        {
                user->SendProtocol(ERR_INPUT, ACCESS_DENIED);
        }
        
        return FAILED;       
}

CommandDelFlags::CommandDelFlags(Module* parent) : Command(parent, "DELFLAG", 2, 2)
{
        flags  = 'r';
        syntax = "<login> <flags>";
}

COMMAND_RESULT CommandDelFlags::Handle(User* user, const Params& parameters)
{
        const std::string& newlogin 	= 	parameters[0];
        const std::string& flag 	= 	parameters[1];
        
        if (flag.empty())
        {
               user->SendProtocol(ERR_INPUT, INVALID_TYPE);
               return FAILED;
        }

        if (flag.length() > 1)
        {
               user->SendProtocol(ERR_INPUT, INVALID_TYPE);
               return FAILED;        
        }

        if (!UserHelper::Exists(newlogin))
        {
               user->SendProtocol(ERR_INPUT, PROCESS_NULL);
               return FAILED;             
        }

        if (newlogin == "root")
        {
               user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
               return FAILED;
        }
        
        const bool exists = UserHelper::HasFlags(newlogin);
        
        if (!exists)
        {
               user->SendProtocol(ERR_NO_FLAGS, PROCESS_NULL);
               return FAILED;
        }
        
        for (auto it = flag.cbegin() ; it != flag.cend(); ++it) 
        {
                if (*it != 'e' && *it != 'r' && *it != 'm')
                {
                        user->SendProtocol(ERR_INPUT, INVALID_TYPE);
                        return FAILED;
                }
        }
        
        std::string adding = UserHelper::CheckFlags(newlogin);
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
        
        if (creating.empty())
        {
             /* We delete the user flags. */
            
             UserHelper::DeleteFlags(newlogin);
             user->SendProtocol(BRLD_OK, PROCESS_OK);
        }
        else
        {
              UserHelper::SetFlags(newlogin, creating);
              user->SendProtocol(BRLD_OK, PROCESS_OK);
        }
        
        /* We notify all users connected about the change */

        //Kernel->Logins->Sessions->NotifyFlags(newlogin, creating);
        return SUCCESS;
}

CommandAddFlag::CommandAddFlag(Module* parent) : Command(parent, "ADDFLAG", 2, 2)
{
        flags  = 'r';
        syntax = "<login> <flag>";
}

COMMAND_RESULT CommandAddFlag::Handle(User* user, const Params& parameters)
{
        const std::string& newlogin 	= 	parameters[0];
        const std::string& flag 	= 	parameters[1];
        
        if (newlogin == "root")
        {
                user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
                return FAILED;
        }
        
        if (flag.length() > 1)
        {
                user->SendProtocol(ERR_INPUT, INVALID_TYPE);
                return FAILED;        
        }
        
        for (auto it = flag.cbegin() ; it != flag.cend(); ++it) 
        {
                if (*it != 'e' && *it != 'r' && *it != 'm')
                {
                        user->SendProtocol(ERR_INPUT, INVALID_TYPE);
                        return FAILED;
                }
        }

        if (!UserHelper::Exists(newlogin))
        {
                user->SendProtocol(ERR_INPUT, PROCESS_NULL);
                return FAILED;             
        }
  
        std::string exists = UserHelper::CheckFlags(newlogin);
        std::string adding;
        
        for (auto it = exists.cbegin() ; it != exists.cend(); ++it) 
        {
                if (*it == flag[0])
                {
                        user->SendProtocol(ERR_INPUT, PROCESS_ALREADY);
                        return FAILED;
                }
                
                adding.push_back(*it);
        }
        
        adding.push_back(flag[0]);

        UserHelper::SetFlags(newlogin, adding);
        Kernel->Logins->Sessions->AttachExternal(newlogin, adding);

        /* We notify all users connected about the change */
        
        Kernel->Logins->Sessions->NotifyFlags(newlogin, adding);
        
        user->SendProtocol(BRLD_OK, PROCESS_OK);
        return SUCCESS;
}

