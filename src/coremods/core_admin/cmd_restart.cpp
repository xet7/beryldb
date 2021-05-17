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
#include "exit.h"
#include "core_admin.h"

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
        requires = 'm';
        syntax = "<servername>";
}

COMMAND_RESULT CommandRestart::Handle(User* user, const Params& parameters)
{
        slog("STARTUP", LOG_DEFAULT, "Shutting down request by " + user->login);
        Kernel->Clients->NotifyAll(BRLD_RESTART, "Restarting server.");

        for (int i = getdtablesize(); --i > 2;)
        {
               int flags = fcntl(i, F_GETFD);
               
               if (flags != -1)
               {
                         fcntl(i, F_SETFD, flags | FD_CLOEXEC);
               }
        }
        
        /* Restarts in next cycle. */
        
        Kernel->Atomics.AddAction(new PromiseRestart());
        return FAILED;
}
