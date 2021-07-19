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

class CommandIsMatch : public Command 
{

    public: 

        CommandIsMatch(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandAsBool : public Command 
{

    public: 

        CommandAsBool(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandIsBool : public Command 
{

    public: 

        CommandIsBool(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

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

class CommandGetOccurs : public Command 
{
    public: 

        CommandGetOccurs(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};



class CommandStrlen : public Command 
{
    public: 

        CommandStrlen(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandKeys : public Command 
{
    public: 

        CommandKeys(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandCount : public Command 
{
    public: 

        CommandCount(Module* Creator);

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

class CommandGetSubstr : public Command 
{
    public: 

        CommandGetSubstr(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandRKey : public Command 
{
    public: 

        CommandRKey(Module* Creator);

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


class CommandWDel : public Command 
{
    public: 

        CommandWDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGetExp : public Command 
{
    public: 

        CommandGetExp(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandIsAlpha : public Command 
{
    public: 

        CommandIsAlpha(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandIsNum : public Command 
{
    public: 

        CommandIsNum(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGetPersist : public Command 
{
    public: 

        CommandGetPersist(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

