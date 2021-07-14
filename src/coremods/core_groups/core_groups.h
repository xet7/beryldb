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

class CommandGroupAdd : public Command 
{
    public: 

        CommandGroupAdd(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGroups : public Command 
{
    public: 

        CommandGroups(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDelGroup : public Command 
{
    public: 

        CommandDelGroup(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGFlags : public Command 
{
    public: 

        CommandGFlags(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMyGroups : public Command 
{
    public: 

        CommandMyGroups(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandAssign : public Command 
{
    public: 

        CommandAssign(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGList : public Command 
{
    public: 

        CommandGList(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGReset : public Command 
{
    public: 

        CommandGReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandUnAssign : public Command 
{
    public: 

        CommandUnAssign(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGShow : public Command 
{
    public: 

        CommandGShow(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandFlagUpdate : public Command 
{
    public: 

        CommandFlagUpdate(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

