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

CommandShutdown::CommandShutdown(Module* parent) : Command(parent, "SHUTDOWN", 0, 1)
{
        flags = 'm';
        syntax = "<servername>";
}

COMMAND_RESULT CommandShutdown::Handle(User* user, const Params& parameters)
{
        std::string reason;
        
        if (parameters.size() > 0)
        {
                reason = parameters.back();
        }
        
        user->SendProtocol(BRLD_OK, PROCESS_OK);
        
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

namespace
{
        struct PromiseRestart : public PromiseAction
        {
                PromiseRestart() 
                {

                }

                void Call() 
                {
                      execvp(Kernel->Config->usercmd.argv[0], Kernel->Config->usercmd.argv);
                      Kernel->Reducer.Apply();
                      Kernel->Reducer.Add(this);
                }
        };
}

CommandRestart::CommandRestart(Module* parent) : Command(parent, "RESTART", 0, 1)
{
        flags = 'm';
        syntax = "<servername>";
}

COMMAND_RESULT CommandRestart::Handle(User* user, const Params& parameters)
{
        slog("STARTUP", LOG_DEFAULT, "Restart request by " + user->login);
        user->SendProtocol(BRLD_OK, PROCESS_OK);
        Kernel->Clients->NotifyAll(BRLD_RESTART, "Restarting server.");

        for (int i = getdtablesize(); --i > 2;)
        {
               int specials = fcntl(i, F_GETFD);
               
               if (specials != -1)
               {
                         fcntl(i, F_SETFD, specials | FD_CLOEXEC);
               }
        }
        
        /* Restarts in next cycle. */
        
        Kernel->Atomics.AddAction(new PromiseRestart());
        return FAILED;
}
