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

class CommandMCount : public Command 
{
    public: 

        CommandMCount(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMSet : public Command 
{
    public: 

        CommandMSet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMGet : public Command 
{
    public: 

        CommandMGet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMKeys : public Command 
{
    public: 

        CommandMKeys(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

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


