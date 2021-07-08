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
#include "managers/keys.h"
#include "managers/globals.h"
#include "managers/databases.h"
#include "engine.h"
#include "core_hints.h"

CommandRename::CommandRename(Module* Creator) : Command(Creator, "RENAME", 2, 2)
{
         syntax = "<key> <new key>";
}

COMMAND_RESULT CommandRename::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& newkey = parameters[1];

       GlobalHelper::Rename(user, key, newkey);
       return SUCCESS;
}

CommandDel::CommandDel(Module* Creator) : Command(Creator, "DEL", 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandDel::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       GlobalHelper::Delete(user, key);
       return SUCCESS;
}

CommandCopy::CommandCopy(Module* Creator) : Command(Creator, "COPY", 2, 2)
{
         syntax = "<key> <dest key>";
}

COMMAND_RESULT CommandCopy::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& destkey = parameters[1];

       GlobalHelper::Copy(user, key, destkey);
       return SUCCESS;
}

CommandExists::CommandExists(Module* Creator) : Command(Creator, "EXISTS", 1, 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandExists::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       GlobalHelper::Exists(user, key);
       return SUCCESS;
}

CommandMove::CommandMove(Module* Creator) : Command(Creator, "MOVE", 2, 2)
{
         syntax = "<key> <select>";
}

COMMAND_RESULT CommandMove::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& new_select = parameters[1];

       if (!is_number(new_select))
       {
                 user->SendProtocol(ERR_USE, DBL_NOT_NUM, MUST_BE_NUMERIC.c_str());
                 return FAILED;
       }

       if (!is_positive_number(new_select))
       {
                user->SendProtocol(ERR_USE, ERR_MUST_BE_POS_INT, MUST_BE_POSIT.c_str());
                return FAILED;
       }

       if (!Daemon::CheckRange(user, new_select, INVALID_RANGE, 1, 100))
       {
               return FAILED;
       }

       GlobalHelper::Move(user, key, new_select);
       return SUCCESS;
}

CommandType::CommandType(Module* Creator) : Command(Creator, "TYPE", 1, 1)
{

}

COMMAND_RESULT CommandType::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       DBHelper::Type(user, key);
       return SUCCESS;
}
