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

class CommandDel : public Command 
{
    public: 

        CommandDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMove : public Command 
{
    public: 

        CommandMove(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandRename : public Command 
{
    public: 

         CommandRename(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandCopy : public Command 
{
    public: 

         CommandCopy(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


/* Checks if a given key exists. */

class CommandExists : public Command 
{
    public: 

        CommandExists(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandClone : public Command 
{
    public: 

        CommandClone(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandType : public Command 
{
    public: 

        CommandType(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandRenameNX : public Command 
{
    public: 

        CommandRenameNX(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
