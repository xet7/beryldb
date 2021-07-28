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
#include "brldb/dbmanager.h"
#include "brldb/dbnumeric.h"
#include "brldb/query.h"

#include "managers/keys.h"

class CommandIncr : public Command 
{
    public: 

        CommandIncr(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandIncrBy : public Command 
{
    public: 

        CommandIncrBy(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDecr : public Command 
{
    public: 

        CommandDecr(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDecrBy : public Command 
{
    public: 

        CommandDecrBy(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandAvg : public Command 
{
    public: 

        CommandAvg(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDiv : public Command 
{
    public: 

        CommandDiv(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMult : public Command 
{
    public: 

        CommandMult(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

