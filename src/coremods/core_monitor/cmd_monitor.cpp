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
                    user->SendProtocol(ERR_INVALID_MONITORLVL, level, Daemon::Format("%s: %s", INVALID_MLEVEL.c_str(), level.c_str())); 
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


CommandMonitorList::CommandMonitorList(Module* Creator) : Command(Creator, "MLIST", 0, 1)
{

}

COMMAND_RESULT CommandMonitorList::Handle(User* user, const Params& parameters)
{
       std::string arg;
        
        if (parameters.size())
        {
              arg = parameters[0];
              std::transform(arg.begin(), arg.end(), arg.begin(), ::toupper);
        }
        else
        {
             arg = "";
        }
        
        if (parameters.size())
        {
             if (arg != "DEFAULT" || arg != "DEBUG")
             {
                  user->SendProtocol(ERR_INVALID_MONITORLVL, arg, Daemon::Format("%s: %s", INVALID_MLEVEL.c_str(), arg.c_str()));
                  return FAILED;
             }
        }
        
        MonitorMap all = Kernel->Monitor->GetList(arg);

        user->SendProtocol(BRLD_MONITOR_LIST, arg, Daemon::Format("BEGIN of MONITOR list.").c_str());
        
        unsigned int counter = 0;

        for (MonitorMap::iterator uit = all.begin(); uit != all.end(); uit++)
        {
               User* umonitor = uit->first;
               MONITOR_LEVEL level = uit->second;
               
               std::string strlevel;
               
               if (level == MONITOR_DEFAULT)
               { 	
                    strlevel = "DEFAULT";
               }
               else if (level == MONITOR_DEBUG)
               {
                    strlevel = "DEBUG";
               }
               
               user->SendProtocol(BRLD_MONITOR_USER, arg, umonitor->instance, Daemon::Format("%s : %s ", umonitor->instance.c_str(), strlevel.c_str()).c_str());
               counter++;
        }
        
        user->SendProtocol(BRLD_MONITOR_LIST, arg, Daemon::Format("BEGIN of MONITOR list (%u)", counter).c_str());
        return SUCCESS;
}