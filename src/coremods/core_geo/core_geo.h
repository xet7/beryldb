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

class CommandGeoAdd : public Command 
{
    public: 

        CommandGeoAdd(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGeoGet : public Command 
{
    public: 

        CommandGeoGet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandGFind : public Command 
{
    public: 

        CommandGFind(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGeoCalc : public Command 
{
    public: 

        CommandGeoCalc(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandGeoDistance : public Command 
{
    public: 

        CommandGeoDistance(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGeoRemove : public Command 
{
    public: 

        CommandGeoRemove(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGeoLoGet : public Command 
{
    public: 

        CommandGeoLoGet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGeoLaGet : public Command 
{
    public: 

        CommandGeoLaGet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
