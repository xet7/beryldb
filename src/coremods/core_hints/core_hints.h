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

#include "managers/keys.h"
#include "managers/globals.h"

/* 
 * Transfers a given key to another database.
 * 
 * @parameters:
 *
 *         · string	: Key to transfer.
 *         · string	: Database destination.
 * 
 * @protocol:
 *
 *         · enum     : OK, FALSE, or ERROR.
 */ 

class CommandTransfer : public Command 
{
    public: 

        CommandTransfer(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Deletes a key from the server.
 * 
 * @parameters:
 *
 *         · string   : key to remove.
 * 
 * @protocol:
 *
 *         · enum     : OK or ERROR.
 */ 
 
class CommandDel : public Command 
{
    public: 

        CommandDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Moves a key to another select.
 * 
 * @parameters:
 *
 *         · string   : Key to move.
 *         · int      : Destination select.
 * 
 * @protocol:
 *
 *         · enum     : OK or ERROR.
 */ 

class CommandMove : public Command 
{
    public: 

        CommandMove(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Renames a key. If key is a simple string and has an expire, it will be
 * changed to the new name.
 * 
 * @parameters:
 *
 *         · string   : Key to rename.
 *         · string   : New key name.
 * 
 * @protocol:
 *
 *         · protocol : NULL, ERROR or OK.
 */

class CommandRename : public Command 
{
    public: 

         CommandRename(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Renames a key, only if dest key has not been defined.
 * 
 * @parameters:
 *
 *         · string   : Key to rename.
 *         · string   : New key name.
 * 
 * @protocol:
 *
 *         · protocol : NULL, ERROR or OK.
 */

class CommandRenameNX : public Command 
{
    public: 

        CommandRenameNX(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Copies a key.
 * 
 * @parameters:
 *
 *         · string   : Key to copy.
 *         · string   : Destination copy name.
 * 
 * @protocol:
 *
 *         · enum     : OK or ERROR.
 */ 

class CommandCopy : public Command 
{
    public: 

         CommandCopy(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Checks whether a given key exists.
 * 
 * @parameters:
 *
 *         · string   : key to check.
 * 
 * @protocol:
 *
 *         · enum     : 1 or 0.
 */ 

class CommandExists : public Command 
{
    public: 

        CommandExists(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Clones (copies a key into another select) a given key.
 * 
 * @parameters:
 *
 *         · string   : key to clone.
 *         · int      : Select to clone key into.
 * 
 * @protocol:
 *
 *         · enum     : NULL, ERROR or ok.
 */ 

class CommandClone : public Command 
{
    public: 

        CommandClone(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Checks the data type for a given key. Possible key types are:
 * INT_LIST, INT_KEY, INT_GEO, INT_MAP, INT_MMAP, INT_VECTOR, 
 * INT_EXPIRE, and INT_FUTURE.
 * 
 * @parameters:
 *
 *         · string   : Keys to check.
 * 
 * @protocol:
 *
 *         · protocol : NULL or ERROR.
 *         · string   : Key type.
 */

class CommandType : public Command 
{
    public: 

        CommandType(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Looks for differences in two keys. Note that keys must be of the same
 * type in order to run this command.
 * 
 * @parameters:
 *
 *         · string   : Key to diff.
 *         · string   : Comparing key.
 * 
 * @protocol:
 *
 *         · enum     : NULL, or ERROR.
 *         · type     : Custom return type, depending on comparing keys.
 */
 
class CommandDiff : public Command 
{
    public: 

        CommandDiff(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Checks whether given keys, as separated by spaces, exist in 
 * the server.
 * 
 * @parameters:
 *
 *         · string   : Keys to check.
 * 
 * @protocol:
 *
 *         · int      : counter.
 */

class CommandTouch : public Command 
{
    public: 

        CommandTouch(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Checks whether given keys, as separated by spaces, do not exist in 
 * the server.
 * 
 * @parameters:
 *
 *         · string   : Keys to check.
 * 
 * @protocol:
 *
 *         · int      : counter.
 */


class CommandNTouch : public Command 
{
    public: 

        CommandNTouch(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

