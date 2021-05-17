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

#pragma once

#include "beryl.h"
#include "modules/message.h"

class CommandHQuery : public Command
{
 public:
        
        CommandHQuery(Module* parent);
        
        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHSend : public Command
{
 public:
        
        CommandHSend(Module* parent);
        
        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHLimits : public Command
{
 public:
        
        CommandHLimits(Module* parent);
        
        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHReset : public Command
{
 public:

        CommandHReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandHParam : public Command
{
 public:

        CommandHParam(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
