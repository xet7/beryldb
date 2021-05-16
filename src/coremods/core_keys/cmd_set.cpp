/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
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
#include "engine.h"
#include "core_keys.h"

CommandSet::CommandSet(Module* Creator) : Command(Creator, "SET", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandSet::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters.back();
       
       if (!Daemon::CheckFormat(user, value))
       {
            return FAILED;
       }
       
       KeyHelper::Set(user, user->current_db, user->select, key, value);
       return SUCCESS;
}

CommandSetNX::CommandSetNX(Module* Creator) : Command(Creator, "SETNX", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandSetNX::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters.back();

       if (!Daemon::CheckFormat(user, value))
       {
            return FAILED;
       }

       KeyHelper::Set(user, user->current_db, user->select, key, value, "", TYPE_SETNX);
       return SUCCESS;
}

CommandSetTX::CommandSetTX(Module* Creator) : Command(Creator, "SETTX", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandSetTX::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters.back();

       if (!Daemon::CheckFormat(user, value))
       {
            return FAILED;
       }

       KeyHelper::Set(user, user->current_db, user->select, key, value, "", TYPE_SETTX);
       return SUCCESS;
}

CommandGetSet::CommandGetSet(Module* Creator) : Command(Creator, "GETSET", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandGetSet::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters.back();

       if (!Daemon::CheckFormat(user, value))
       {
            return FAILED;
       }

       KeyHelper::AdvSet(user, user->current_db, user->select, key, value, TYPE_GETSET);
       return SUCCESS;
}

CommandRename::CommandRename(Module* Creator) : Command(Creator, "RENAME", 2, 2)
{
         syntax = "<key> <new key>";
}

COMMAND_RESULT CommandRename::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& newkey = parameters[1];

       KeyHelper::AdvSet(user, user->current_db, user->select, key, newkey, TYPE_RENAME);
       return SUCCESS;
}

CommandRenameNX::CommandRenameNX(Module* Creator) : Command(Creator, "RENAMENX", 2, 2)
{
         syntax = "<key> <new key>";
}

COMMAND_RESULT CommandRenameNX::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& newkey = parameters[1];

       KeyHelper::AdvSet(user, user->current_db, user->select, key, newkey, TYPE_RENAMENX);
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

       KeyHelper::AdvSet(user, user->current_db, user->select, key, destkey, TYPE_COPY);
       return SUCCESS;
}

CommandAppend::CommandAppend(Module* Creator) : Command(Creator, "APPEND", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandAppend::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters.back();

       KeyHelper::Append(user, user->current_db, user->select, key, value);
       return SUCCESS;
}