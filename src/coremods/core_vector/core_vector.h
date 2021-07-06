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

class CommandVGet : public Command 
{
    public: 

        CommandVGet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVPush : public Command 
{

    public: 

        CommandVPush(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVExist : public Command 
{
    public: 

        CommandVExist(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVCount : public Command 
{
    public: 

        CommandVCount(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVPos : public Command 
{
    public: 

        CommandVPos(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVResize : public Command 
{
    public: 

        CommandVResize(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVKeys : public Command 
{
    public: 

        CommandVKeys(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVPopFront : public Command 
{
    public: 

        CommandVPopFront(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVPopBack : public Command 
{
    public: 

        CommandVPopBack(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVDel : public Command 
{
    public: 

        CommandVDel(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVEraseFrom : public Command 
{
    public: 

        CommandVEraseFrom(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVReverse : public Command 
{
    public: 

        CommandVReverse(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandVRepeats : public Command 
{
    public: 

        CommandVRepeats(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
