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
#include "monitor.h"
#include "core_monitor.h"

CommandStopMonitor::CommandStopMonitor(Module* Creator) : Command(Creator, "STOPMONITOR", 0, 0)
{
        requires = 'm';
}

COMMAND_RESULT CommandStopMonitor::Handle(User* user, const Params& parameters)
{  
       if (!Kernel->Monitor->Has(user))
       {
               user->SendProtocol(ERR_NO_MONITOR, PROCESS_FALSE);
               return FAILED;
       }
       
       Kernel->Monitor->Remove(user);
       user->SendProtocol(BRLD_STOP_MONITOR, PROCESS_OK);          
       return SUCCESS;
}
