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

class CommandHGetAll : public Command 
{
    public: 

        CommandHGetAll(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * HSet defines a map in the server.
 * 
 * @parameters:
 *
 *         · string: Key to define this map with.
 *         · string: Hash to use.
 *         · string: Value to associate this hash with.
 * 
 * @protocol:
 *
 *         · enum: OK or ERROR.
 */ 

class CommandHSet : public Command 
{
    public: 

        CommandHSet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * HSet defines a map in the server, only if NOT previously defined.
 * 
 * @parameters:
 *
 *         · string: Key to define this map with.
 *         · string: Hash to use.
 *         · string: Value to associate this hash with.
 * 
 * @protocol:
 *
 *         · enum: OK or ERROR.
 */ 
class CommandHSetNX : public Command
{
    public: 

        CommandHSetNX(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * HGet retrieves a keys from a map-hash.
 * 
 * @parameters:
 *
 *         · string: Key map to look keys in.
 *         · string: Maps' hesh.
 * 
 * @protocol:
 *
 *         · enum: OK or ERROR.
 */ 
class CommandHGet : public Command 
{
    public: 

        CommandHGet(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * HDel deletes a hash.
 * 
 * @parameters:
 *
 *         · string: Key associated with hash to remove.
 *         · hash: Removing hash.
 * 
 * @protocol:
 *
 *         · enum: OK or ERROR.
 */ 
 
class CommandHDel : public Command 
{
    public: 

        CommandHDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Counts element in a map.
 * 
 * @parameters:
 *
 *         · string   : Key to count.
 * 
 * @protocol:
 *
 *         · protocol : NULL, or ERROR.
 *         · int      : Count.
 */

class CommandHCount : public Command 
{
    public: 

        CommandHCount(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * HKeys looks for all maps defined in the server.
 * This command allows wildcards.
 * 
 * @parameters:
 *
 *         · string  : Pattern to lookup.
 * 
 * @protocol:
 *
 *         · vector  : maps founds.
 */ 

class CommandHKeys : public Command 
{
    public: 

        CommandHKeys(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * HLIST returns all keys for a given map.
 * 
 * @parameters:
 *
 *         · string  : Map to find.
 * 
 * @protocol:
 *
 *         · vector  : keys founds.
 */ 
 
class CommandHList : public Command 
{
    public: 

        CommandHList(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * HExists checks whether a given hash (key) exists.
 * 
 * @parameters:
 *
 *         · string  : Map to find.
 * 
 * @protocol:
 *
 *         · bool    : 1 or 0.
 */ 
 
class CommandHExists : public Command 
{
    public: 

        CommandHExists(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * HStrlen returns a key's length.
 * 
 * @parameters:
 *
 *         · string   : Key to find.
 *         · string   : Hash to obtain strlen.
 * 
 * @protocol:
 *
 *         · enum     : OK or ERROR.
 *         · int      : Length of given hash.
 */ 

class CommandHStrlen : public Command
{
    public: 

        CommandHStrlen(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);

};

class CommandHVals : public Command
{
    public: 

        CommandHVals(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);

};
