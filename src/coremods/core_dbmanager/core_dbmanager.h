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
#include "brldb/dbmanager.h"
#include "engine.h"

class CommandCurrent : public Command 
{
    public: 

        CommandCurrent(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandPWD : public Command 
{
    public: 

        CommandPWD(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandUse : public Command 
{
    public: 

        CommandUse(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* Flushing datbase requires root flags. */

class CommandFlushDB : public Command 
{
    public: 

        CommandFlushDB(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* Returns database size. */

class CommandDBSize : public Command 
{
    public: 

        CommandDBSize(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDBReset : public Command 
{
    public: 
    
        CommandDBReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandSFlush : public Command 
{
    public: 

        CommandSFlush(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandUsing : public Command 
{
    public: 

        CommandUsing(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandSwapDB : public Command 
{
    public: 

        CommandSwapDB(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
