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
#include "notifier.h"
#include "converter.h"
#include "maker.h"

#include "managers/expires.h"

/* 
 * Expire command sets an expire on a given key.
 * 
 * In order to use this command, provided key MUST be defined.
 * Finally, this command may be used even when a key is already
 * expiring.
 * 
 * @parameters:
 *
 *         · string	: Key to expire.
 *         · int	: Seconds from current time to make a given key expire.
 * 
 * @protocol:
 *
 *         · protocol   : ERROR or OK.
 */

class CommandExpire : public Command 
{
    public: 

        CommandExpire(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * ExpireAT command sets an expire  on a given key. 
 * 
 * @parameters:
 *
 *         · string     : Key to expire.
 *         · int        : Unix timestamp to set this expire on.
 * 
 * @protocol:
 *
 *         · protocol   : ERROR or OK.
 */

class CommandExpireAT : public Command 
{
    public: 

        CommandExpireAT(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Setex sets a new key, and adds an expire on top of it.
 * 
 * @parameters:
 *
 *         · int        : Seconds from current time to make a given key expire.
 *         · string     : Key to set. 
 *         · string     : Value to add.
 * 
 * @protocol:
 *
 *         · protocol   : ERROR or OK.
 */

class CommandSetex : public Command 
{
    public: 

        CommandSetex(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns time to live for a given key.
 * 
 * @parameters:
 *
 *         · string     : Key to check.
 * 
 * @protocol:
 *
 *         · protocol   : NULL.
 *         · int        : Seconds before key expires.
 */

class CommandTTL : public Command 
{
    public: 

        CommandTTL(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns time to live for a given key, as expressed in unix timestamp.
 * 
 * @parameters:
 *
 *         · string     : Key to check.
 * 
 * @protocol:
 *
 *         · protocol   : NULL.
 *         · int        : Timestamp before key expires.
 */

class CommandTTLAT : public Command 
{
    public: 

        CommandTTLAT(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Removes an expire on a key, without not removing the entry itself.
 * 
 * @parameters:
 *
 *         · string     : Key to persist.
 * 
 * @protocol:
 *
 *         · protocol   : NULL.
 */

class CommandPersist : public Command 
{
    public: 

        CommandPersist(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Finds all expires in current select and database, as in a given pattern.
 * 
 * @parameters:
 *
 *         · string     : Key to lookup.
 *         · char       : h (human readable) or r (raw).
 * 
 * @protocol:
 *
 *         · map        : All expires found.
 */

class CommandExpireFIND : public Command 
{
    public: 

        CommandExpireFIND(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Lists all expires across all databases and selects.
 * 
 * @parameters:
 *
 *         · char       : h (human readable) or r (raw).
 * 
 * @protocol:
 *
 *         · map        : All expires found.
 */

class CommandExpireLIST : public Command 
{
    public: 

        CommandExpireLIST(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Removes all expires in all databases.
 * 
 * @requires 'e'.
 *
 * @protocol:
 *
 *         · protocol   : OK.
 */

class CommandReset : public Command 
{
    public: 

        CommandReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Removes all expires in current select.
 * 
 * @requires 'e'.
 *
 * @protocol:
 *
 *         · protocol   : OK.
 */
 
class CommandSelectReset : public Command 
{
    public: 

        CommandSelectReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns all expires in a given select.
 *
 * @parameters:
 *
 *         · int   : Current select, if no select is provided,
 *                   expires in current select will be returned. 
 *
 * @protocol:
 *
 *         · map   : Expires as keyed in a map.
 */

class CommandSelectCount : public Command 
{
    public: 

        CommandSelectCount(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDBEReset : public Command 
{
    public: 

        CommandDBEReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
