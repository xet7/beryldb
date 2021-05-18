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

class CoreModuleMonitor : public Module
{
    private:
    
        CommandMonitor cmdmonitor;
        CommandStopMonitor cmdstopmonitor;
        CommandMonitorReset cmdmonitoreset;
        CommandMonitorList cmdmonitorlist;

    public:     
        
        CoreModuleMonitor() : cmdmonitor(this), cmdstopmonitor(this), cmdmonitoreset(this),
                              cmdmonitorlist(this)

        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides monitor-related functions.", VF_BERYLDB);
        }
};

MODULE_LOAD(CoreModuleMonitor)

