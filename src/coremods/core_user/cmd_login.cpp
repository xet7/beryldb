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
#include "core_user.h"
#include "managers/settings.h"
#include "brldb/dbmanager.h"

CommandLogin::CommandLogin(Module* parent) : MultiCommand(parent, "LOGIN", 1, 1)
{
	pre_reg_ok = true;
	no_hint_until_reg = true;
	syntax = "<login>";
}

COMMAND_RESULT CommandLogin::HandleLocal(LocalUser* user, const Params& parameters)
{
	const std::string& newlogin = parameters[0];

	if (newlogin.empty())
	{
		user->SendProtocol(ERR_INPUT2, ERR_NO_LOG_PROV, NO_LOG_PROV);
		return FAILED;
	}

	if (!Kernel->Engine->ValidLogin(newlogin))
	{
		user->SendProtocol(ERR_INPUT2, ERR_INVALID_LOGIN, ERRON_LOGIN);
		return FAILED;
	}

	ModuleResult MOD_RESULT;
	UNTIL_RESULT(OnUserPreLogin, MOD_RESULT, (user, newlogin));

	if (MOD_RESULT == MOD_RES_STOP)
	{
		return FAILED;
	}

	if (!user->SetLogin(newlogin))
	{
		return FAILED;
	}

        /* Default database assignation. */

        const std::string dbuser = STHelper::Get("dbuser", newlogin);
        
        if (!dbuser.empty())
        {
                std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbuser);

                if (!database)
                {
	                user->SetDatabase(Kernel->Store->GetDefault());
                }
                else
                {
	                user->SetDatabase(database);
                }
        }
        else
        {
                user->SetDatabase(Kernel->Store->GetDefault());
        }

        if (user->registered < REG_LOGINUSER)
        {
                user->registered = (user->registered | REG_LOGIN);
                return CommandAgent::CheckRegister(user);
        }

	return SUCCESS;
}
