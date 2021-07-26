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
#include "core_notifier.h"

CommandNotifier::CommandNotifier(Module* Creator) : Command(Creator, "NOTIFY", 0, 1)
{
       requires = 'm';
       syntax = "<level>";
}

COMMAND_RESULT CommandNotifier::Handle(User* user, const Params& parameters)
{      
       Kernel->Notify->Remove(user);
       
       if (parameters.size())
       {
             std::string level = parameters[0];
             std::transform(level.begin(), level.end(), level.begin(), ::toupper);
       
             NOTIFY_LEVEL monitor = NOTIFY_DEFAULT;
             
             if (level == "DEFAULT")
             {
                    monitor = NOTIFY_DEFAULT;
             }
             else if (level == "VERBOSE")
             {
                    monitor = NOTIFY_VERBOSE;
             }
             else if (level == "DEBUG")
             {
                    monitor = NOTIFY_DEBUG;
             }
             else
             {
                    user->SendProtocol(ERR_INPUT2, ERR_INVALID_NOTLVL, Daemon::Format("%s:%s", INVALID_NLEVEL.c_str(), level.c_str())); 
                    return FAILED;
             }
       
             STHelper::Set("notify", user->login, level);

             Kernel->Notify->Add(monitor, user);
             user->SendProtocol(BRLD_OK, PROCESS_OK);
             return SUCCESS;  
       }

       STHelper::Set("notify", user->login, "DEFAULT");
       Kernel->Notify->Add(NOTIFY_DEFAULT, user);
       user->SendProtocol(BRLD_OK, PROCESS_OK);          
       return SUCCESS;
}

CommandNotifyReset::CommandNotifyReset(Module* Creator) : Command(Creator, "NRESET", 0, 0)
{
        requires = 'm';
}

COMMAND_RESULT CommandNotifyReset::Handle(User* user, const Params& parameters)
{
       Kernel->Notify->Reset();
       STHelper::Erase("notify");
       user->SendProtocol(BRLD_OK, PROCESS_OK);
       return SUCCESS;
}

CommandStopNotify::CommandStopNotify(Module* Creator) : Command(Creator, "STOPNOTIFY", 0, 0)
{
        requires = 'm';
}

COMMAND_RESULT CommandStopNotify::Handle(User* user, const Params& parameters)
{  
       if (!Kernel->Notify->Has(user))
       {
               user->SendProtocol(ERR_NO_NOTIFY, PROCESS_FALSE);
               return FAILED;
       }
       
       STHelper::Delete("notify", user->login);       
       Kernel->Notify->Remove(user);
       user->SendProtocol(BRLD_OK, PROCESS_OK);          
       return SUCCESS;
}


