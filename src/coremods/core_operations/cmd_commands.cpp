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
#include "core_operations.h"

CommandIncr::CommandIncr(Module* Creator) : Command(Creator, "INCR", 1)
{
         group = 'k';
         syntax = "<key>";
}

COMMAND_RESULT CommandIncr::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];

        if (!CheckKey(user, key))
        {
            return FAILED;
        }

        KeyHelper::Operation(user, key, OP_INCR);
        return SUCCESS;  
}

CommandIncrBy::CommandIncrBy(Module* Creator) : Command(Creator, "INCRBY", 2, 2)
{
         group = 'k';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandIncrBy::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters[1];
        
        if (!CheckKey(user, key))
        {
            return FAILED;
        }

        if (!is_number(value, true))
        {
                user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
                return FAILED;
        }

        KeyHelper::Operation(user, key, OP_ADD, value);
        return SUCCESS;  
}

CommandAvg::CommandAvg(Module* Creator) : Command(Creator, "AVG", 2, 2)
{
         group = 'k';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandAvg::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters[1];
        
        if (!CheckKey(user, key))
        {
            return FAILED;
        }

        if (!is_number(value, true))
        {
                user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
                return FAILED;
        }

        KeyHelper::Operation(user, key, OP_AVG, value);
        return SUCCESS;  
}

CommandDecr::CommandDecr(Module* Creator) : Command(Creator, "DECR", 1)
{
         group = 'k';
         syntax = "<key>";
}

COMMAND_RESULT CommandDecr::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];

        KeyHelper::Operation(user, key, OP_DECR);
        return SUCCESS;  
}

CommandDecrBy::CommandDecrBy(Module* Creator) : Command(Creator, "DECRBY", 2, 2)
{
         group = 'k';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandDecrBy::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters[1];

        if (!is_number(value, true))
        {
                user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
                return FAILED;
        }

        KeyHelper::Operation(user, key, OP_MIN, value);
        return SUCCESS;  
}

CommandDiv::CommandDiv(Module* Creator) : Command(Creator, "DIV", 2, 2)
{
         group = 'k';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandDiv::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters[1];

        if (!CheckKey(user, key))
        {
            return FAILED;
        }
        
        if (!is_number(value, true))
        {
                user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
                return FAILED;
        }

        KeyHelper::Operation(user, key, OP_DIV, value);
        return SUCCESS; 
}

CommandMult::CommandMult(Module* Creator) : Command(Creator, "MULT", 2, 2)
{
         group = 'k';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandMult::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters[1];

        if (!CheckKey(user, key))
        {
            return FAILED;
        }
        
        if (!is_number(value, true))
        {
                user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
                return FAILED;
        }

        KeyHelper::Operation(user, key, OP_MULT, value);
        return SUCCESS; 
}

CommandSqrt::CommandSqrt(Module* Creator) : Command(Creator, "SQRT", 1, 1)
{
         group = 'k';
         syntax = "<key>";
}

COMMAND_RESULT CommandSqrt::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];

        if (!CheckKey(user, key))
        {
            return FAILED;
        }

        KeyHelper::Operation(user, key, OP_SQRT, "NULL");
        return SUCCESS; 
}
