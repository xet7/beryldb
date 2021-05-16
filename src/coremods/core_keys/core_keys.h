/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
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

class CommandSet : public Command 
{

    public: 

        CommandSet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandSetTX : public Command 
{
    public: 

        CommandSetTX(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandSetNX : public Command 
{
    public: 

        CommandSetNX(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandGet : public Command 
{
    public: 

        CommandGet(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDel : public Command 
{
    public: 

        CommandDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandStrlen : public Command 
{
    public: 

        CommandStrlen(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandFind : public Command 
{
    public: 

        CommandFind(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandCount : public Command 
{
    public: 

        CommandCount(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMove : public Command 
{
    public: 

        CommandMove(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* Checks if a given key exists. */

class CommandExists : public Command 
{
    public: 

        CommandExists(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class  CommandGetDel : public Command 
{
    public: 

         CommandGetDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class  CommandGetSet : public Command 
{
    public: 

         CommandGetSet(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class  CommandGetRange : public Command 
{
    public: 

         CommandGetRange(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandRename : public Command 
{
    public: 

         CommandRename(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandRenameNX : public Command 
{
    public: 

        CommandRenameNX(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandCopy : public Command 
{
    public: 

         CommandCopy(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandRKey : public Command 
{
    public: 

        CommandRKey(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandTouch : public Command 
{
    public: 

        CommandTouch(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandNTouch : public Command 
{
    public: 

        CommandNTouch(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandTCount : public Command 
{
    public: 

        CommandTCount(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandConcat : public Command 
{
    public: 

        CommandConcat(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandSConcat : public Command 
{
    public: 

        CommandSConcat(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandAppend : public Command 
{
    public: 

        CommandAppend(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandSearch : public Command 
{
    public: 

        CommandSearch(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

