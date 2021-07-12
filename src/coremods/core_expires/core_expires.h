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

class CommandExpire : public Command 
{
    public: 

        CommandExpire(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandTTL : public Command 
{
    public: 

        CommandTTL(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandTTLAT : public Command 
{
    public: 

        CommandTTLAT(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandSetex : public Command 
{
    public: 

        CommandSetex(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandPersist : public Command 
{
    public: 

        CommandPersist(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandExpireLIST : public Command 
{
    public: 

        CommandExpireLIST(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandReset : public Command 
{
    public: 

        CommandReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandExpireAT : public Command 
{
    public: 

        CommandExpireAT(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandSelectReset : public Command 
{
    public: 

        CommandSelectReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandSelectCount : public Command 
{
    public: 

        CommandSelectCount(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDBEReset : public Command 
{
    public: 

        CommandDBEReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
