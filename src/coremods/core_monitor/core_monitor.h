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
#include "engine.h"
#include "monitor.h"

#include "brldb/dbmanager.h"

/* 
 * Monitor handles the 'monitor' command, which is used to
 * activate monitoring handling.
 * 
 * Monitoring levels are: DEFAULT and DEBUG.
 * 
 * @parameters:
 *
 *         · string	: level.
 * 
 * @protocol:
 *
 *         · enum	: OK or ERROR.
 */ 
 
class CommandMonitor : public Command 
{
    public: 

        CommandMonitor(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * StopMonitor is a command that allows a user to stop receiving
 * monitoring data. 
 *
 * @protocol:
 *
 *         · OK		: Client will no longer monitor.
 *         · ERROR	: Clients is not currently monitoring.
 */
 
class CommandStopMonitor : public Command 
{
    public: 

        CommandStopMonitor(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Monitor reseting results in all subscribed clients to the monitoring
 * map, to be automatically unsubscribed from monitor streams.
 * 
 * @protocol:
 *
 *         · enum	: OK.
 */
 
class CommandMonitorReset : public Command 
{
    public: 

        CommandMonitorReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * MonitorList lists all active clients receiving monitoring alerts.
 * 
 * @requires 'm'.
 *
 * @parameters:
 *
 *         · DEFAULT or DEBUG	: Wether to list debug or default clients.
 * 
 * @protocol:
 *
 *         · enum		: OK or ERROR.
 */

class CommandMonitorList : public Command 
{
    public: 

        CommandMonitorList(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

