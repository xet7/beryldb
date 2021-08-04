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
#include "core_monitor.h"

CommandMonitor::CommandMonitor(Module* Creator) : Command(Creator, "MONITOR", 0, 1)
{
         requires = 'm';
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
                    user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
                    return FAILED;
             }
       
             Kernel->Monitor->Add(user, monitor);
             user->SendProtocol(BRLD_OK, PROCESS_OK);       
             return SUCCESS;  
       }
       
       Kernel->Monitor->Add(user, MONITOR_DEFAULT);
       user->SendProtocol(BRLD_OK, PROCESS_OK);          
       return SUCCESS;
}

CommandMonitorReset::CommandMonitorReset(Module* Creator) : Command(Creator, "MRESET", 0, 0)
{
        requires = 'm';
}

COMMAND_RESULT CommandMonitorReset::Handle(User* user, const Params& parameters)
{
       Kernel->Monitor->Reset();
       user->SendProtocol(BRLD_OK, PROCESS_OK);
       return SUCCESS;
}


CommandMonitorList::CommandMonitorList(Module* Creator) : Command(Creator, "MONITORLIST", 0, 1)
{
        requires = 'm';
        syntax = "<*argument>";
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
                  user->SendProtocol(ERR_INPUT, INVALID_MLEVEL);
                  return FAILED;
             }
        }
        
        const MonitorMap& all = Kernel->Monitor->GetList(arg);
        
        Dispatcher::JustAPI(user, BRLD_START_LIST);
        Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s | %-10s", "Monitor", "Level"));
        Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s | %-10s", Dispatcher::Repeat("―", 30).c_str(), Dispatcher::Repeat("―", 10).c_str()));
        
        unsigned int counter = 0;

        for (MonitorMap::const_iterator uit = all.begin(); uit != all.end(); uit++)
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
               
               Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", umonitor->instance.c_str(), strlevel.c_str()), Daemon::Format("%s %s", umonitor->instance.c_str(), strlevel.c_str()));
               counter++;
        }
        
        Dispatcher::JustAPI(user, BRLD_END_LIST);
        return SUCCESS;
}

CommandStopMonitor::CommandStopMonitor(Module* Creator) : Command(Creator, "STOPMONITOR", 0, 0)
{
        requires = 'm';
}

COMMAND_RESULT CommandStopMonitor::Handle(User* user, const Params& parameters)
{  
       if (!Kernel->Monitor->Has(user))
       {
               user->SendProtocol(ERR_INPUT, PROCESS_FALSE);
               return FAILED;
       }
       
       Kernel->Monitor->Remove(user);
       user->SendProtocol(BRLD_OK, PROCESS_OK);          
       return SUCCESS;
}
