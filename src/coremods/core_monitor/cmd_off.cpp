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

CommandStopMonitor::CommandStopMonitor(Module* Creator) : Command(Creator, "STOPMONITOR", 0, 0)
{

}

COMMAND_RESULT CommandStopMonitor::Handle(User* user, const Params& parameters)
{  
       if (!Kernel->Monitor->Has(user))
       {
               user->SendProtocol(ERR_NO_MONITOR, "No longer monitoring.");
       }
       
       Kernel->Monitor->Remove(user);
       user->SendProtocol(BRLD_STOP_MONITOR, "Monitor stopped.");          
       return SUCCESS;
}
