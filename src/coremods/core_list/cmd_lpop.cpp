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
#include "core_list.h"

CommandLPopBack::CommandLPopBack(Module* Creator) : Command(Creator, "LPOPBACK", 1, 1)
{
        check_key	=	0;
        group 		= 	'l';
        syntax 		= 	"<key>";
}

COMMAND_RESULT CommandLPopBack::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Retro(user, std::make_shared<lpop_back_query>(), parameters[0]);
        return SUCCESS;  
}

CommandLPopFront::CommandLPopFront(Module* Creator) : Command(Creator, "LPOPFRONT", 1, 1)
{
        check_key       =       0;
        group 		= 	'l';
        syntax 		= 	"<key>";
}

COMMAND_RESULT CommandLPopFront::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Retro(user, std::make_shared<lpop_front_query>(), parameters[0]);
        return SUCCESS;  
}

CommandPopAll::CommandPopAll(Module* Creator) : Command(Creator, "LPOPALL", 2, 2)
{
        check_value     =       true;
        check_key       =       0;
        group 		= 	'l';
        syntax 		= 	"<key> \"value\"";
}

COMMAND_RESULT CommandPopAll::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Simple(user, std::make_shared<lpopall_query>(), parameters[0], parameters.back());
        return SUCCESS;  
}

CommandLReverse::CommandLReverse(Module* Creator) : Command(Creator, "LREVERSE", 1, 1)
{
        check_key       =       0;
        group 		= 	'l';
        syntax 		= 	"<key>";
}

COMMAND_RESULT CommandLReverse::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Retro(user, std::make_shared<lreverse_query>(), parameters[0]);
        return SUCCESS;  
}

CommandLSort::CommandLSort(Module* Creator) : Command(Creator, "LSORT", 1, 1)
{
        check_key       =       0;
        group 		= 	'l';
        syntax 		= 	"<key>";
}

COMMAND_RESULT CommandLSort::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Retro(user, std::make_shared<lsort_query>(), parameters[0]);
        return SUCCESS;  
}

CommandLDel::CommandLDel(Module* Creator) : Command(Creator, "LDEL", 2, 2)
{
        check_value     =       true;
        check_key       =       0;
        group 		= 	'l';
        syntax 		= 	"<key> \"value\"";
}

COMMAND_RESULT CommandLDel::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Simple(user, std::make_shared<ldel_query>(), parameters[0], parameters.back());
        return SUCCESS;  
}
