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

class CommandLPush : public Command 
{

    public: 

        CommandLPush(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLGet : public Command 
{
    public: 

        CommandLGet(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandPop : public Command 
{
    public: 

        CommandPop(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandPopAll : public Command 
{
    public: 

        CommandPopAll(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLExist : public Command 
{
    public: 

        CommandLExist(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLCount : public Command 
{
    public: 

        CommandLCount(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLMove : public Command 
{
    public: 

        CommandLMove(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandLPos : public Command 
{
    public: 

        CommandLPos(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandLSearch : public Command 
{
    public: 

        CommandLSearch(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLFind : public Command 
{
    public: 

        CommandLFind(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLRemove : public Command 
{
    public: 

        CommandLRemove(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

