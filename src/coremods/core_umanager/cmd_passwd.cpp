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
#include "engine.h"
#include "core_umanager.h"
#include "managers/user.h"
#include "brldb/database.h"

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
                        user->SendProtocol(ERR_INPUT2, ERR_INVALID_PASS, PASS_AT_LEAST);
                        return FAILED;
                }

                return SUCCESS;
        }
        
        /* Check if user has root access. */
        
        if (!user->IsAdmin())
        {
                user->SendProtocol(ERR_INPUT2, ERR_NO_FLAGS, ACCESS_DENIED);
                return FAILED;        
        }
                
        const std::string& newlogin = parameters[0];
        const std::string& pass = parameters[1];

        if (newlogin.length() < 3 || newlogin.length() > 15)
        {
                user->SendProtocol(ERR_INPUT2, ERR_LOGIN_EXISTS, USER_MIMMAX_LENGTH);
                return FAILED;
        }

        if (!Kernel->Engine->ValidLogin(newlogin))
        {
                user->SendProtocol(ERR_INPUT2, ERR_INVALID_LOGIN, INVALID_UNAME);
                return FAILED;
        }
        
        if (pass.length() < 3 || pass.length() > 30)
        {
                user->SendProtocol(ERR_INPUT2, ERR_INVALID_PASS, PASS_MIMMAX_LENGTH);
                return FAILED; 
        }
        
        std::string exists = UserHelper::Find(newlogin, "created");
        
        if (exists.empty())
        {
                user->SendProtocol(ERR_INPUT2, ERR_LOGIN_NO_EXISTS, PROCESS_FALSE);
                return FAILED;
        }
        
        if (UserHelper::ChangePass(newlogin, pass))
        {
                user->SendProtocol(BRLD_OK, PROCESS_OK);
        }
        else
        {
                user->SendProtocol(ERR_INPUT2, ERR_INVALID_PASS, PASS_AT_LEAST);
                return FAILED;
        }
        
        return SUCCESS;
}

CommandResetCache::CommandResetCache(Module* parent) : Command(parent, "RESETCACHE", 0)
{
         requires = 'r';
}

COMMAND_RESULT CommandResetCache::Handle(User* user, const Params& parameters)
{
        Kernel->Logins->Reset();
        
        user->SendProtocol(BRLD_OK, PROCESS_OK);
        return SUCCESS;
}

CommandInCache::CommandInCache(Module* parent) : Command(parent, "INCACHE", 1, 1)
{
         requires = 'r';
         syntax = "<login>";
}

COMMAND_RESULT CommandInCache::Handle(User* user, const Params& parameters)
{
        const std::string& login = parameters[0];

        user->SendProtocol(BRLD_OK, convto_string(Kernel->Logins->InCache(login)));
        return SUCCESS;
}
