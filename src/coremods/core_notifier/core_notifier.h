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

#include "managers/settings.h"

/* 
 * NOTIFY commands enables notifying. Notifications are
 * special alerts that the server sends to users with 'm' flag or
 * superior.
 *
 * @requires 'm'.
 * 
 * @parameters:
 *
 *         · string  : Notify level: DEFAULT, VERBOSE or DEBUG.
 * 
 * @protocol:
 *
 *         · enum    : OK or ERROR.
 */ 

class CommandNotifier : public Command 
{
    public: 

        CommandNotifier(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Remove an user from receiving notifications.
 *
 * @protocol:
 *
 *         · enum    : OK or FALSE.
 */ 

class CommandStopNotify : public Command 
{
    public: 

        CommandStopNotify(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Removes all users from receiving notifications.
 * 
 * @protocol:
 *
 *         · enum    : OK.
 */ 

class CommandNotifyReset : public Command 
{
    public: 

        CommandNotifyReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

