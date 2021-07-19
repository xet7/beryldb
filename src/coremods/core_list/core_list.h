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

class CommandLPopFront : public Command 
{
    public: 

        CommandLPopFront(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLPopBack : public Command 
{
    public: 

        CommandLPopBack(Module* Creator);

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

class CommandLResize : public Command 
{
    public: 

        CommandLResize(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandLKeys : public Command 
{
    public: 

        CommandLKeys(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLFind : public Command 
{
    public: 

        CommandLFind(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLReverse : public Command 
{
    public: 

        CommandLReverse(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLSort : public Command 
{
    public: 

        CommandLSort(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLPos : public Command 
{
    public: 

        CommandLPos(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLDel : public Command 
{
    public: 

        CommandLDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLRepeats : public Command 
{
    public: 

        CommandLRepeats(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLRop : public Command 
{
    public: 

        CommandLRop(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandFRop : public Command 
{
    public: 

        CommandFRop(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLBack : public Command 
{

    public: 

        CommandLBack(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandLFront : public Command 
{
    public: 

        CommandLFront(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

