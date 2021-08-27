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

#include "brldb/query.h"
#include "managers/keys.h"

/* 
 * Increments a key by 1.
 * 
 * @parameters:
 *
 *         · string   : Key to increment.
 * 
 * @protocol:
 *
 *         · protocol : NULL, or ERROR.
 *         · double   : New value.
 */

class CommandIncr : public Command 
{
    public: 

        CommandIncr(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Increments a key by a given number.
 * 
 * @parameters:
 *
 *         · string   : Key to increment.
 *         · double   : Value to increment key by.
 * 
 * @protocol:
 *
 *         · protocol : NULL, or ERROR.
 *         · double   : New value.
 */
 
class CommandIncrBy : public Command 
{
    public: 

        CommandIncrBy(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Decrements a key by 1.
 * 
 * @parameters:
 *
 *         · string   : Key to decrement.
 * 
 * @protocol:
 *
 *         · protocol : NULL, or ERROR.
 *         · double   : New value.
 */
 
class CommandDecr : public Command 
{
    public: 

        CommandDecr(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Decrements a key by a given number.
 * 
 * @parameters:
 *
 *         · string   : Key to decrement.
 *         · double   : Value to decrement. key by.
 * 
 * @protocol:
 *
 *         · protocol : NULL, or ERROR.
 *         · double   : New value.
 */

class CommandDecrBy : public Command 
{
    public: 

        CommandDecrBy(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Averages a key with a value.
 * 
 * @parameters:
 *
 *         · string   : Key to average.
 *         · double   : Value to average.
 * 
 * @protocol:
 *
 *         · protocol : NULL, or ERROR.
 *         · double   : New value.
 */

class CommandAvg : public Command 
{
    public: 

        CommandAvg(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Divide a key with a value.
 * 
 * @parameters:
 *
 *         · string   : Key to divide.
 *         · double   : Value to divide.
 * 
 * @protocol:
 *
 *         · protocol : NULL, or ERROR.
 *         · double   : New value.
 */

class CommandDiv : public Command 
{
    public: 

        CommandDiv(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Multiplies a key with a value.
 * 
 * @parameters:
 *
 *         · string   : Key to multiply.
 *         · double   : Value to multiply.
 * 
 * @protocol:
 *
 *         · protocol : NULL, or ERROR.
 *         · double   : New value.
 */

class CommandMult : public Command 
{
    public: 

        CommandMult(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Squares root a key.
 * 
 * @parameters:
 *
 *         · string   : Key to square root.
 *         · double   : Value to square root.
 * 
 * @protocol:
 *
 *         · protocol : NULL, or ERROR.
 *         · double   : New value.
 */

class CommandSqrt : public Command 
{
    public: 

        CommandSqrt(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

