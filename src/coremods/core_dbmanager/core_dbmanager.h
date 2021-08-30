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
#include "converter.h"
#include "notifier.h"
#include "maker.h"
#include "extras.h"
#include "engine.h"

#include "managers/databases.h"
#include "managers/keys.h"
#include "managers/settings.h"
#include "managers/globals.h"
#include "managers/tests.h"

/* 
 * Returns current select in use.
 * 
 * @protocol:
 *
 *         · int        : current select.
 */

class CommandCurrent : public Command 
{
    public: 

        CommandCurrent(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns current working directory.
 * 
 * @protocol:
 *
 *         · string     : Full working directory.
 */

class CommandPWD : public Command 
{
    public: 

        CommandPWD(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Changes current select.
 * 
 * @parameters:
 *
 *         · use	: New select.
 * 
 * @protocol:
 *
 *         · enum	: OK, ERROR, INVALID_RANGE.
 */ 

class CommandUse : public Command 
{
    public: 

        CommandUse(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Flushes current database.
 * 
 * @requires 'r'.
 * 
 * @protocol:
 *
 *         · enum	:  OK.
 */

class CommandFlushDB : public Command 
{
    public: 

        CommandFlushDB(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Flushes current select.
 * 
 * @requires 'r'.
 * 
 * @protocol:
 *
 *         · enum       :  OK.
 */

class CommandSFlush : public Command 
{
    public: 

        CommandSFlush(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns database size.
 * If no parameter is provided, current database is checked.
 * 
 * @parameters:
 *
 *         · database name: Database to find size of.
 * 
 * @protocol:
 *
 *         · size: Database size.
 */ 

class CommandDBSize : public Command 
{
    public: 

        CommandDBSize(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns using database for a given instance.
 * 
 * @requires 'm'.
 *
 * @parameters:
 *
 *         · string     : Instance to look for.
 * 
 * @protocol:
 *
 *         · enum       : NOT FOUND.
 *         · int        : select database found.
 */

class CommandUsing : public Command 
{
    public: 

        CommandUsing(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returrns current database in use.
 * 
 * @protocol:
 *
 *         · string : current database.
 */

class CommandDB : public Command 
{
    public: 

        CommandDB(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Changes current database in use.
 * 
 * @parameters:
 *      
 *         · string   : New database to use.
 *
 * @protocol:
 *
 *         · protocol : NULL or OK.
 */

class CommandChange : public Command 
{
    public: 

        CommandChange(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Lists all databases in system. This command returns dblist in the Name, Path format.
 *
 * @requires 'r'.
 * 
 * @protocol:
 *
 *         · map        :  Databases map.
 */

class CommandDBLIST : public Command 
{
    public: 

        CommandDBLIST(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Creates a new database.
 * 
 * @requires 'r'.
 *
 * @parameters:
 *
 *         · string     : Database name.
 *         · string     : Database path.
 * 
 * @protocol:
 *
 *         · enum       : OK or FALSE.
 */

class CommandDBCreate : public Command 
{
    public: 

        CommandDBCreate(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Deletes a database.
 * 
 * @requires 'r'.
 *
 * @parameters:
 *
 *         · string     : Database to delete.
 * 
 * @protocol:
 *
 *         · enum       : OK or FALSE.
 */

class CommandDBDelete : public Command 
{
    public: 

        CommandDBDelete(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Prints all content in database. This command should only be used
 * when the server is running with --nofork.
 */

class CommandDBTest : public Command 
{
    public: 

        CommandDBTest(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns Default database.
 * 
 * @protocol:
 *
 *         · string     : Database name.
 */

class CommandDefault : public Command 
{
    public: 

        CommandDefault(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Defines default database to use.
 * 
 * @requires 'r'.
 *
 * @protocol:
 *
 *         · enum        : OK.
 */

class CommandDBSetDefault : public Command 
{
    public: 

        CommandDBSetDefault(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Removes all data in all databases and selects.
 * 
 * @requires 'r'.
 *
 * @protocol:
 *
 *         · enum        : OK.
 */

class CommandFlushAll : public Command 
{
    public: 

        CommandFlushAll(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

