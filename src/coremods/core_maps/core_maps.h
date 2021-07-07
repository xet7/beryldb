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

class CommandHGetAll : public Command 
{
    public: 

        CommandHGetAll(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHSet : public Command 
{
    public: 

        CommandHSet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHGet : public Command 
{
    public: 

        CommandHGet(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHDel : public Command 
{
    public: 

        CommandHDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};



class CommandHCount : public Command 
{
    public: 

        CommandHCount(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHFind : public Command 
{
    public: 

        CommandHFind(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHList : public Command 
{
    public: 

        CommandHList(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHExists : public Command 
{
    public: 

        CommandHExists(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHStrlen : public Command
{
    public: 

        CommandHStrlen(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);

};

class CommandHSetNX : public Command
{
    public: 

        CommandHSetNX(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHClone : public Command
{
    public: 

        CommandHClone(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);

};

class CommandHWDel : public Command
{
    public: 

        CommandHWDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);

};

class CommandHVals : public Command
{
    public: 

        CommandHVals(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);

};
