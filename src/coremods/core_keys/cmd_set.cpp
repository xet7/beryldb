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
         group = 'k';
         syntax = "<key> <value>";
         group = 'k';
}

COMMAND_RESULT CommandSet::Handle(User* user, const Params& parameters)
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
       
       KeyHelper::Set(user, key, value);
       return SUCCESS;
}

CommandSetNX::CommandSetNX(Module* Creator) : Command(Creator, "SETNX", 2, 2)
{
         group = 'k';
         syntax = "<key> <value>";
         group = 'k';
}

COMMAND_RESULT CommandSetNX::Handle(User* user, const Params& parameters)
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

       KeyHelper::SetNX(user, key, value);
       return SUCCESS;
}

CommandSetTX::CommandSetTX(Module* Creator) : Command(Creator, "SETTX", 2, 2)
{
         groups = { 'e', 'k' };
         syntax = "<key> <value>";
         groups = { 'e', 'k' };
}

COMMAND_RESULT CommandSetTX::Handle(User* user, const Params& parameters)
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

       KeyHelper::SetTX(user, key, value);
       return SUCCESS;
}

CommandGetSet::CommandGetSet(Module* Creator) : Command(Creator, "GETSET", 2, 2)
{
         group = 'k';
         syntax = "<key> <value>";
         group = 'k';
}

COMMAND_RESULT CommandGetSet::Handle(User* user, const Params& parameters)
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

       KeyHelper::GetSet(user, key, value);
       return SUCCESS;
}


CommandAppend::CommandAppend(Module* Creator) : Command(Creator, "APPEND", 2, 2)
{
         group = 'k';
         syntax = "<key> <value>";
         group = 'k';
}

COMMAND_RESULT CommandAppend::Handle(User* user, const Params& parameters)
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

       KeyHelper::Append(user, key, value);
       return SUCCESS;
}

CommandCount::CommandCount(Module* Creator) : Command(Creator, "COUNT", 0, 1)
{
<<<<<<< HEAD
         syntax = "<\%key>";
         group = 'k';
=======
       group = 'k';
       syntax = "<\%key>";
>>>>>>> unstable
}

COMMAND_RESULT CommandCount::Handle(User* user, const Params& parameters)
{  
       std::string key;

       /* We use a wildcardcard if no key is provided. */
       
       if (!parameters.size())
       {
            key = "*";
       }
       else
       {
             key = parameters[0];
       }
       
       KeyHelper::Count(user, key);
       return SUCCESS;
}

