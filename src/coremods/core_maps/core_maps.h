/*
 * BerylDB - A modular database.
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

class CommandHSET : public Command 
{

    public: 

        CommandHSET(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHGET : public Command 
{
    public: 

        CommandHGET(Module* Creator);

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

class CommandHSearch : public Command 
{
    public: 

        CommandHSearch(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHDelAll : public Command 
{
    public: 

        CommandHDelAll(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandHSeek : public Command 
{
    public: 

        CommandHSeek(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandHKeys : public Command 
{
    public: 

        CommandHKeys(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHExists : public Command 
{
    public: 

        CommandHExists(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHMove : public Command
{
    public: 

        CommandHMove(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);

};