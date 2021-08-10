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

#pragma once

#include "beryl.h"
#include "engine.h"


class CommandMulti : public Command 
{

    public: 

        CommandMulti(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMRUN : public Command 
{

    public: 

        CommandMRUN(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandMultiReset : public Command 
{

    public: 

        CommandMultiReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

