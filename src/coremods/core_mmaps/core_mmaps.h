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

/* 
 * MCOUNT counts items in a map.
 * 
 * @parameters:
 *
 *         · string     : Multimap name.
 * 
 * @protocol:
 *
 *         · enum       : NULL or ERROR.
 *         · int        : Item count.
 */ 
 
class CommandMCount : public Command 
{
    public: 

        CommandMCount(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * MPUSH pushes a new element to a multimap.
 * 
 * @parameters:
 *
 *         · string	: Multimap name.
 *         · string	: Value to add.
 * 
 * @protocol:
 *
 *         · enum	: OK or ERROR.
 */ 

class CommandMPush : public Command 
{
    public: 

        CommandMPush(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * MPUSHNX pushes a new element to a multimap, only if adding element
 * does not exists.
 * 
 * @parameters:
 *
 *         · string     : Multimap name.
 *         · string     : Value to add.
 * 
 * @protocol:
 *
 *         · enum       : OK or ERROR.
 */ 

class CommandMPushNX : public Command 
{
    public: 

        CommandMPushNX(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * MGET returns all keys for a given multimap.
 * 
 * @parameters:
 *
 *         · string     : Multimap name.
 * 
 * @protocol:
 *
 *         · enum       : NULL or ERROR.
 *         · map        : Results.
 */ 

class CommandMGet : public Command 
{
    public: 

        CommandMGet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * MKEYS returns all keys for a given wildcard.
 * 
 * @parameters:
 *
 *         · string     : Wildcard to use.
 * 
 * @protocol:
 *
 *         · Vector     : Results.
 */ 
 
class CommandMKeys : public Command 
{
    public: 

        CommandMKeys(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * MDEL deletes a given item from a map.
 * 
 * @parameters:
 *
 *         · string     : Multimap name.
 *         · string     : Value to remove.
 * 
 * @protocol:
 *
 *         · enum       : NULL, OK or ERROR.
 */ 

class CommandMDel : public Command 
{
    public: 

        CommandMDel(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMSeek : public Command 
{
    public: 

        CommandMSeek(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMRepeats : public Command 
{
    public: 

        CommandMRepeats(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMVals : public Command 
{
    public: 

        CommandMVals(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * MGETALL returns all keys and values for a given multimap.
 * 
 * @parameters:
 *
 *         · string     : Multimap name.
 * 
 * @protocol:
 *
 *         · enum       : NULL or ERROR.
 *         · map        : Results.
 */ 
 
class CommandMGetAll : public Command 
{
    public: 

        CommandMGetAll(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandMIter : public Command 
{
    public: 

        CommandMIter(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

