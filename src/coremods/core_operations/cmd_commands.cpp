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
        check_key       =       0;
        group 		= 	'k';
        syntax 		= 	"<key>";
}

COMMAND_RESULT CommandIncr::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Operation(user, parameters[0], OP_INCR);
        return SUCCESS;  
}

CommandIncrBy::CommandIncrBy(Module* Creator) : Command(Creator, "INCRBY", 2, 2)
{
        check_key       =       0;
        check_num       =       1;
        group 		= 	'k';
        syntax 		= 	"<key> <value>";
}

COMMAND_RESULT CommandIncrBy::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Operation(user, parameters[0], OP_ADD, parameters[1]);
        return SUCCESS;  
}

CommandAvg::CommandAvg(Module* Creator) : Command(Creator, "AVG", 2, 2)
{
        check_key       =       0;
        check_num	=	1;
        group 		= 	'k';
        syntax 		=	"<key> <value>";
}

COMMAND_RESULT CommandAvg::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Operation(user, parameters[0], OP_AVG, parameters[1]);
        return SUCCESS;  
}

CommandDecr::CommandDecr(Module* Creator) : Command(Creator, "DECR", 1)
{
        check_key       =       0;
        group 		= 	'k';
        syntax 		= 	"<key>";
}

COMMAND_RESULT CommandDecr::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Operation(user, parameters[0], OP_DECR);
        return SUCCESS;  
}

CommandDecrBy::CommandDecrBy(Module* Creator) : Command(Creator, "DECRBY", 2, 2)
{
        check_key       =       0;
        check_num       =       1;
        group 		= 	'k';
        syntax 		= 	"<key> <value>";
}

COMMAND_RESULT CommandDecrBy::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Operation(user, parameters[0], OP_MIN, parameters[1]);
        return SUCCESS;  
}

CommandDiv::CommandDiv(Module* Creator) : Command(Creator, "DIV", 2, 2)
{
        check_key       =       0;
        check_num       =       1;
        group 		= 	'k';
        syntax 		= 	"<key> <value>";
}

COMMAND_RESULT CommandDiv::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Operation(user, parameters[0], OP_DIV, parameters[1]);
        return SUCCESS; 
}

CommandMult::CommandMult(Module* Creator) : Command(Creator, "MULT", 2, 2)
{
        check_key       =       0;
        check_num       =       1;
        group 		= 	'k';
        syntax 		= 	"<key> <value>";
}

COMMAND_RESULT CommandMult::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Operation(user, parameters[0], OP_MULT, parameters[1]);
        return SUCCESS; 
}

CommandSqrt::CommandSqrt(Module* Creator) : Command(Creator, "SQRT", 1, 1)
{
        check_key       =       0;
        group 		= 	'k';
        syntax 		= 	"<key>";
}

COMMAND_RESULT CommandSqrt::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Operation(user, parameters[0], OP_SQRT, "NULL");
        return SUCCESS; 
}
