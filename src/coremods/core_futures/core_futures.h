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

class CommandFuture : public Command 
{
    public: 

        CommandFuture(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandTTE : public Command 
{
    public: 

        CommandTTE(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandExec : public Command 
{
    public: 

        CommandExec(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandFReset : public Command 
{
    public: 

        CommandFReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandCancel : public Command 
{
    public: 

        CommandCancel(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandFResetAll : public Command 
{
    public: 

        CommandFResetAll(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandFutureList : public Command 
{
    public: 

        CommandFutureList(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandFutureAT : public Command 
{
    public: 

        CommandFutureAT(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandSelectCount : public Command 
{
    public: 

        CommandSelectCount(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

