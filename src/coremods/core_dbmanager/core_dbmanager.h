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
#include "engine.h"

#include "brldb/dbmanager.h"
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "brldb/database.h"

#include "managers/databases.h"
#include "managers/settings.h"
#include "managers/globals.h"
#include "managers/tests.h"

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


class CommandDB : public Command 
{
    public: 

        CommandDB(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandChange : public Command 
{
    public: 

        CommandChange(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDBLIST : public Command 
{
    public: 

        CommandDBLIST(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDBCreate : public Command 
{
    public: 

        CommandDBCreate(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDBDelete : public Command 
{
    public: 

        CommandDBDelete(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandDBTest : public Command 
{
    public: 

        CommandDBTest(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDefault : public Command 
{
    public: 

        CommandDefault(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDBSetDefault : public Command 
{
    public: 

        CommandDBSetDefault(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandFlushAll : public Command 
{
    public: 

        CommandFlushAll(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

