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

/* 
 * Creates a multi. Multi allows commands to be ran in a sequence.
 * 
 * @protocol:
 *
 *         · protocol        : OK or ERROR.
 */ 
 
class CommandMulti : public Command 
{

    public: 

        CommandMulti(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Flushes elements added to a multi.
 *
 * @protocol:
 *
 *         · protocol        : OK or ERROR.
 */ 
 
class CommandMRUN : public Command 
{

    public: 

        CommandMRUN(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Resets a multi.
 * 
 * @protocol:
 *
 *         · protocol        : OK or ERROR.
 */ 

class CommandMultiReset : public Command 
{

    public: 

        CommandMultiReset(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

