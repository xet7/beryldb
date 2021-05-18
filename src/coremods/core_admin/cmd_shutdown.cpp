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
#include "exit.h"
#include "core_admin.h"

CommandShutdown::CommandShutdown(Module* parent) : Command(parent, "SHUTDOWN", 0, 1)
{
        requires = 'm';
        syntax = "<servername>";
}

COMMAND_RESULT CommandShutdown::Handle(User* user, const Params& parameters)
{
        std::string reason;
        
        if (parameters.size() > 0)
        {
                reason = parameters.back();
        }
        
        if (!reason.empty())
        {
                slog("STARTUP", LOG_DEFAULT, Daemon::Format("Shutting down request by %s: %s", user->login.c_str(), reason.c_str()));
                Kernel->Exit(EXIT_CODE_SHUTDOWN, false, false, Daemon::Format("Shutting down: %s", reason.c_str()).c_str());
        }
        else
        {
                slog("STARTUP", LOG_DEFAULT, "Shutting down request by " + user->login);
                Kernel->Exit(EXIT_CODE_SHUTDOWN, false, false, "Shutting down.");
        }
        
        return SUCCESS;
}
