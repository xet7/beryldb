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
#include "core_list.h"

CommandLExist::CommandLExist(Module* Creator) : Command(Creator, "LEXIST", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandLExist::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];

       if (!Daemon::CheckFormat(user, value))
       {
           return FAILED;
       }
       
       ////ListHelper::Exist(user, Kernel->Store->GetDefault(), user->select, key, value);
       return SUCCESS;  
}

CommandLCount::CommandLCount(Module* Creator) : Command(Creator, "LCOUNT", 1, 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandLCount::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
//       //ListHelper::Exist(user, Kernel->Store->GetDefault(), user->select, key, "", TYPE_COUNT_RECORDS);
       return SUCCESS;  
}

CommandLPos::CommandLPos(Module* Creator) : Command(Creator, "LPOS", 3, 3)
{
         syntax = "<position> <key> <value>";
}

COMMAND_RESULT CommandLPos::Handle(User* user, const Params& parameters)
{  
       const std::string& index = parameters[0];
       const std::string& key = parameters[1];
       const std::string& value = parameters[2];

       if (!Daemon::CheckFormat(user, value))
       {
           return FAILED;
       }
       
       unsigned int n_index = convto_num<unsigned int>(index);
   //    //ListHelper::Exist(user, Kernel->Store->GetDefault(), user->select, key, value, TYPE_LPOS, n_index);
       return SUCCESS;  
}

