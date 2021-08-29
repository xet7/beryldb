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
#include "extras.h"
#include "managers/keys.h"

/* 
 * Returns given char in a string.
 * 
 * @parameters:
 *
 *         · string   : Key to find.
 *         · int      : Char position.
 * 
 * @protocol:
 *
 *         · enum     : NULL, ERROR or OK.
 */

class CommandChar : public Command 
{
    public: 

        CommandChar(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Capitalizes a given string.
 * 
 * @parameters:
 *
 *         · string   : Key to capitalize.
 * 
 * @protocol:
 *
 *         · enum     : NULL, ERROR or OK.
 */

class CommandToCap : public Command 
{
    public: 

        CommandToCap(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Transform an string to uppercase.
 * 
 * @parameters:
 *
 *         · string   : Key to transform.
 * 
 * @protocol:
 *
 *         · enum     : NULL, ERROR or OK.
 */

class CommandToUpper : public Command 
{
    public: 

        CommandToUpper(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Transform an string to lowercase.
 * 
 * @parameters:
 *
 *         · string   : Key to transform.
 * 
 * @protocol:
 *
 *         · enum     : NULL, ERROR or OK.
 */

class CommandToLower : public Command 
{
    public: 

        CommandToLower(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Inserts a string into a entry in the database.
 * 
 * @parameters:
 *
 *         · string   : Value to add.
 *         · int      : Where to insert.
 * 
 * @protocol:
 *
 *         · protocol : NULL, ERROR or OK.
 */

class CommandInsert : public Command 
{
    public: 

        CommandInsert(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Checks wether a string maches a given pattern.
 * This commands allows wildcards.
 * 
 * @parameters:
 *
 *         · string   : Key to search.
 *         · string   : Value to match.
 * 
 * @protocol:
 *
 *         · bool     : True or false.
 */

class CommandIsMatch : public Command 
{

    public: 

        CommandIsMatch(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns a given key as expressed in bool. Keep in mind that if a key is not 'true',
 * 0, 1, no, yes, true or false, this command will always return 'false' as response. 
 *
 * @parameters:
 *
 *         · string   : Key to check.
 * 
 * @protocol:
 *
 *         · protocol : ERROR or NULL.
 *         · string   : true or false.
 */

class CommandAsBool : public Command 
{
    public: 

        CommandAsBool(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Checks whether a string is a boolean or not.
 * 
 * @parameters:
 *
 *         · string   : Key to check.
 * 
 * @protocol:
 *
 *         · protocol : ERROR or NULL.
 *         · bool     : true or false.
 */

class CommandIsBool : public Command 
{
    public: 

        CommandIsBool(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Sets a new key.
 * 
 * @parameters:
 *
 *         · string   : Key to define.
 *         · string   : Value this key will have.
 * 
 * @protocol:
 *
 *         · protocol : OK or ERROR.
 */

class CommandSet : public Command 
{
    public: 

        CommandSet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Sets a new key only if it does not has any timer set on it.
 * 
 * @parameters:
 *
 *         · string   : Key to define.
 *         · string   : Value this key will have.
 * 
 * @protocol:
 *
 *         · protocol : OK or ERROR.
 */

class CommandSetTX : public Command 
{
    public: 

        CommandSetTX(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Sets a new key, only if not previosuly defined.
 * 
 * @parameters:
 *
 *         · string   : Key to define.
 *         · string   : Value this key will have.
 * 
 * @protocol:
 *
 *         · protocol : OK or ERROR.
 */

class CommandSetNX : public Command 
{
    public: 

        CommandSetNX(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Gets a key.
 * 
 * @parameters:
 *
 *         · string   : Key to retrieve.
 * 
 * @protocol:
 *
 *         · protocol : NULL or ERROR.
 *         · string   : Obtained key.
 */

class CommandGet : public Command 
{
    public: 

        CommandGet(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Verifies whether an string has repeated strings.
 * 
 * @parameters:
 *
 *         · string   : Key to check.
 *         · string   : Pattern to find.
 * 
 * @protocol:
 *
 *         · string   : String to match.
 *         · protocol : NULL, or ERROR.
 */
 
class CommandGetOccurs : public Command 
{
    public: 

        CommandGetOccurs(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Counts the length of a string.
 * 
 * @parameters:
 *
 *         · string   : Entry to count length of.
 * 
 * @protocol:
 *
 *         · int      : sized of string.
 *         · protocol : Not found.
 */

class CommandStrlen : public Command 
{
    public: 

        CommandStrlen(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Finds a pattern in all keys in a given select and database.
 * This command allows wildcards.
 * 
 * @parameters:
 *
 *         · string  : Key to find.
 * 
 * @protocol:
 *
 *         · vector  : List of keys that matched pattern.
 */

class CommandKeys : public Command 
{
    public: 

        CommandKeys(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Counts all matching keys in a system.
 * This command allows wildcards.
 * 
 * @parameters:
 *
 *         · string   : Key to match.
 * 
 * @protocol:
 *
 *         · enum     : NULL or ERROR.
 *         · vector   : Matching keys.
 */

class CommandCount : public Command 
{
    public: 

        CommandCount(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * GETDEL retrieves and then deletes a key.
 * 
 * @parameters:
 *
 *         · string   : Key to get and delete.
 * 
 * @protocol:
 *
 *         · enum     : NULL or ERROR.
 *         · string   : Key in the system.
 */

class  CommandGetDel : public Command 
{
    public: 

         CommandGetDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * GETSET retrieves and then sets a new value on a key.
 * 
 * @parameters:
 *
 *         · string   : Key to get and re-set.
 *         · value    : New value to this key.
 * 
 * @protocol:
 *
 *         · enum    : NULL or ERROR.
 *         · string  : Key in the system.
 */

class  CommandGetSet : public Command 
{
    public: 

         CommandGetSet(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Changes the sequence of a string in the server.
 * 
 * @parameters:
 *
 *         · string     :  Key to modify.
 *         · { offset, limit }
 *
 * @protocol:
 *
 *        · string	: Modified string.
 */

class CommandGetSubstr : public Command 
{
    public: 

        CommandGetSubstr(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Random key command.
 * 
 * @protocol:
 *
 *         · string	:  Random key retrieved.
 */

class CommandRKey : public Command 
{
    public: 

        CommandRKey(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Appends a string to an entry.
 * 
 * @parameters:
 *
 *         · string   : Key to modify.
 *         · string   : Value to append at the end of entry.
 * 
 * @protocol:
 *
 *         · protocol : NULL, ERROR or OK.
 */

class CommandAppend : public Command 
{
    public: 

        CommandAppend(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Searches for a key. This command allows wildcards.
 * 
 * @parameters:
 *
 *         · string   : Key to search.
 * 
 * @protocol:
 *
 *         · vector   : List of matched items.
 */

class CommandSearch : public Command 
{
    public: 

        CommandSearch(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Removes a range of matching keys. 
 * 
 * @parameters:
 *
 *         · string   : Wildcard to remove.
 * 
 * @protocol:
 *
 *         · int      : Counter (removed keys).
 */

class CommandWDel : public Command 
{
    public: 

        CommandWDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * GETEXP retrieves a key and then sets an expire on it.
 *
 * @parameters:
 *
 *         · int      : Seconds before expiring.
 *         · string   : Key to check.
 * 
 * @protocol:
 *
 *         · protocol : OK, NULL or ERROR.
 */

class CommandGetExp : public Command 
{
    public: 

        CommandGetExp(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Checks whether a given key is alpha numeric.
 * 
 * @parameters:
 *
 *         · string   : Key to check.
 * 
 * @protocol:
 *
 *         · protocol : NULL or ERROR.
 *         · bool     : True or false.
 */

class CommandIsAlpha : public Command 
{
    public: 

        CommandIsAlpha(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Checks whether a given key is numeric.
 * 
 * @parameters:
 *
 *         · string   : Key to check.
 * 
 * @protocol:
 *
 *         · protocol : NULL or ERROR.
 *         · bool     : True or false.
 */

class CommandIsNum : public Command 
{
    public: 

        CommandIsNum(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Obtains a key (GET) and then removes an expires (if any).
 * 
 * @parameters:
 *
 *         · string   : Key to get/persist.
 * 
 * @protocol:
 *
 *         · protocol : NULL, ERROR or OK.
 */
 
class CommandGetPersist : public Command 
{
    public: 

        CommandGetPersist(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

