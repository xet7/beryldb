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

class CommandMonitor : public Command 
{

    public: 

        CommandMonitor(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandStopMonitor : public Command 
{

    public: 

        CommandStopMonitor(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMonitorReset : public Command 
{

    public: 

        CommandMonitorReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMonitorList : public Command 
{

    public: 

        CommandMonitorList(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

