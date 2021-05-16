/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"
#include "monitor.h"
#include "core_monitor.h"

CommandMonitor::CommandMonitor(Module* Creator) : Command(Creator, "MONITOR", 0, 1)
{
         syntax = "<level>";
}

COMMAND_RESULT CommandMonitor::Handle(User* user, const Params& parameters)
{      
       /* We remove in case this user is already monitoring. */
       
       Kernel->Monitor->Remove(user);
       
       if (parameters.size())
       {
             std::string level = parameters[0];
             std::transform(level.begin(), level.end(), level.begin(), ::toupper);
       
             MONITOR_LEVEL monitor = MONITOR_DEFAULT;
             
             if (level == "DEFAULT")
             {
                    monitor = MONITOR_DEFAULT;
             }
             else if (level == "DEBUG")
             {
                    monitor = MONITOR_DEBUG;
             }
             else
             {
                    user->SendProtocol(ERR_WRONG_MONITOR, level, Daemon::Format("Incorrect monitor: %s", level.c_str())); 
                    return FAILED;
             }
       
             Kernel->Monitor->Add(user, monitor);
             user->SendProtocol(BRLD_NOW_MONITORING, level, Daemon::Format("Monitor activated: %s", level.c_str()));       
             return SUCCESS;  
       }
       
       Kernel->Monitor->Add(user, MONITOR_DEFAULT);
       user->SendProtocol(BRLD_NOW_MONITORING, "DEFAULT", "Monitor activated: Default");          
       return SUCCESS;
}

CommandMonitorReset::CommandMonitorReset(Module* Creator) : Command(Creator, "MRESET", 0, 0)
{

}

COMMAND_RESULT CommandMonitorReset::Handle(User* user, const Params& parameters)
{
     unsigned int count = Kernel->Monitor->Count();
     
     Kernel->Monitor->Reset();
     user->SendProtocol(BRLD_RESET_MONITOR, count, Daemon::Format("Monitor reset: %u", count).c_str());
     return SUCCESS;
}