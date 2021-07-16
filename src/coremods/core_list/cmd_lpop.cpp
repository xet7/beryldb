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
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "managers/lists.h"
#include "engine.h"
#include "maker.h"
#include "core_list.h"

CommandLPopBack::CommandLPopBack(Module* Creator) : Command(Creator, "LPOPBACK", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandLPopBack::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        
        if (!CheckKey(user, key))
        {
            return FAILED;
        }

        ListHelper::PopBack(user, key);
        return SUCCESS;  
}

CommandLPopFront::CommandLPopFront(Module* Creator) : Command(Creator, "LPOPFRONT", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandLPopFront::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
   
        if (!CheckKey(user, key))
        {
             return FAILED;
        }
     
        ListHelper::PopFront(user, key);
        return SUCCESS;  
}

CommandPopAll::CommandPopAll(Module* Creator) : Command(Creator, "LPOPALL", 2, 2)
{
         group = 'l';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandPopAll::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters.back();

        if (!CheckKey(user, key))
        {
            return FAILED;
        }

        if (!CheckFormat(user, value))
        {
             return FAILED;
        }

        ListHelper::PopAll(user, key, value);
        return SUCCESS;  
}

CommandLReverse::CommandLReverse(Module* Creator) : Command(Creator, "LREVERSE", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandLReverse::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];

        if (!CheckKey(user, key))
        {
            return FAILED;
        }

        ListHelper::Reverse(user, key);
        return SUCCESS;  
}

CommandLSort::CommandLSort(Module* Creator) : Command(Creator, "LSORT", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandLSort::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];

        if (!CheckKey(user, key))
        {
            return FAILED;
        }

        ListHelper::Sort(user, key);
        return SUCCESS;  
}

CommandLDel::CommandLDel(Module* Creator) : Command(Creator, "LDEL", 2, 2)
{
         group = 'l';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandLDel::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters.back();
    
        if (!CheckKey(user, key))
        {
            return FAILED;
        }
    
        if (!CheckFormat(user, value))
        {
             return FAILED;
        }

        ListHelper::Del(user, key, value);
        return SUCCESS;  
}
