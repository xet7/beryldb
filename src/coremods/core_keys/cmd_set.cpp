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
#include "engine.h"
#include "maker.h"
#include "core_keys.h"

CommandSet::CommandSet(Module* Creator) : Command(Creator, "SET", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandSet::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters.back();
       
       if (!CheckFormat(user, value))
       {
            return FAILED;
       }
       
       KeyHelper::Set(user, key, value);
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

       if (!CheckFormat(user, value))
       {
            return FAILED;
       }

       KeyHelper::SetNX(user, key, value);
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

       if (!CheckFormat(user, value))
       {
            return FAILED;
       }

       KeyHelper::SetTX(user, key, value);
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

       if (!CheckFormat(user, value))
       {
            return FAILED;
       }

       KeyHelper::GetSet(user, key, value);
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

       if (!CheckFormat(user, value))
       {
            return FAILED;
       }

       KeyHelper::Append(user, key, value);
       return SUCCESS;
}