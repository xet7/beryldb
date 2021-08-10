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
#include "maker.h"

#include "managers/expires.h"
#include "managers/globals.h"

#include "engine.h"
#include "converter.h"

/* 
 * Future creates a new future.
 * 
 * @parameters:
 *
 *         · string     : Key to utilize.
 *         · int        : Seconds to define for this future.
 *         · string     : Value to define upon executing time.
 * 
 * @protocol:
 *
 *         · protocol   : ERROR or OK.
 */

class CommandFuture : public Command 
{
    public: 

        CommandFuture(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * FutureAT adds a new future as specified in epoch time.
 * 
 * @parameters:
 *
 *         · int        : Seconds in epoch to define for this future.
 *         · string     : Future key.
 *         · string     : Value to define upon executing time.
 * 
 * @protocol:
 *
 *         · protocol   : ERROR or OK.
 */

class CommandFutureAT : public Command 
{
    public: 

        CommandFutureAT(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Time to execution returns the time remaining for execution on
 * a given future.
 * 
 * @parameters:
 *
 *         · string     : Key to check.
 * 
 * @protocol:
 *
 *         · protocol   : ERROR or OK.
 */
 
class CommandTTE : public Command 
{
    public: 

        CommandTTE(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Executes a future before its due time.
 * 
 * @parameters:
 *
 *         · string     : Key to execute.
 * 
 * @protocol:
 *
 *         · protocol   : ERROR or OK.
 */

class CommandExec : public Command 
{
    public: 

        CommandExec(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandFReset : public Command 
{
    public: 

        CommandFReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Cancels a future.
 * 
 * @parameters:
 *
 *         · string  : Future key to cancel.
 * 
 * @protocol:
 *
 *         · enum    : NULL, ERROR or OK.
 */
 
class CommandCancel : public Command 
{
    public: 

        CommandCancel(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandFResetAll : public Command 
{
    public: 

        CommandFResetAll(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Future List, as expressed by Key, Schedule, Select and Database.
 * 
 * @parameters:
 *
 *         · string  : Argument (human or raw formats).
 * 
 * @protocol:
 *
 *         · vector    : Future list.
 */

class CommandFutureList : public Command 
{
    public: 

        CommandFutureList(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns all futures in current seelct.
 * 
 * @protocol:
 *
 *         · vector    : Future list.
 */

class CommandSelectCount : public Command 
{
    public: 

        CommandSelectCount(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

