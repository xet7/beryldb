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
#include "core_umanager.h"
#include "managers/user.h"
#include "brldb/database.h"

CommandChangePass::CommandChangePass(Module* parent) : Command(parent, "CHANGEPASS", 1, 2)
{
        syntax = "<password>";
}

COMMAND_RESULT CommandChangePass::Handle(User* user, const Params& parameters)
{
        if (parameters.size() < 2)
        {
                const std::string& pass = parameters[0];

                if (UserHelper::ChangePass(user->login, pass))
                {
                        user->SendProtocol(BRLD_LOGIN_CHPASS, user->login, PASS_CHANGED);
                }
                else
                {
                        user->SendProtocol(ERR_INVALID_PASS, pass, PASS_AT_LEAST);
                        return FAILED;
                }

                return SUCCESS;
        }
        
        /* Check if user has root access. */
        
        if (!user->IsAdmin())
        {
                user->SendProtocol(ERR_NO_FLAGS, user->login, ACCESS_DENIED);
                return FAILED;        
        }
                
        const std::string& newlogin = parameters[0];
        const std::string& pass = parameters[1];

        if (newlogin.length() < 3 || newlogin.length() > 15)
        {
                user->SendProtocol(ERR_LOGIN_EXISTS, USER_MIMMAX_LENGTH);
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
        
        std::string exists = UserHelper::Find(newlogin, "created");
        
        if (exists.empty())
        {
                user->SendProtocol(ERR_LOGIN_NO_EXISTS, "Login does not exists.");
                return FAILED;
        }
        
        if (UserHelper::ChangePass(newlogin, pass))
        {
                user->SendProtocol(BRLD_LOGIN_CHPASS, newlogin, PASS_CHANGED);
        }
        else
        {
                user->SendProtocol(ERR_INVALID_PASS, pass, PASS_AT_LEAST);
                return FAILED;
        }

        return SUCCESS;
}

