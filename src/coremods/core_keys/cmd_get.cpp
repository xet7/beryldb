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
#include "engine.h"
#include "maker.h"
#include "core_keys.h"

CommandGetOccurs::CommandGetOccurs(Module* Creator) : Command(Creator, "OCCURS", 2, 2)
{
         group = 'k';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandGetOccurs::Handle(User* user, const Params& parameters)
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

       KeyHelper::GetOccurs(user, key, value);
       return SUCCESS;
}

CommandGet::CommandGet(Module* Creator) : Command(Creator, "GET", 1, 1)
{
         group = 'k';
         syntax = "<key>";
}

COMMAND_RESULT CommandGet::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       KeyHelper::Get(user, key);
       return SUCCESS;
}


CommandStrlen::CommandStrlen(Module* Creator) : Command(Creator, "STRLEN", 1)
{
         group = 'k';
         syntax = "<key>";
}

COMMAND_RESULT CommandStrlen::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       KeyHelper::Strlen(user, key);
       return SUCCESS;
}

CommandGetDel::CommandGetDel(Module* Creator) : Command(Creator, "GETDEL", 1, 1)
{
         group = 'k';
         syntax = "<key>";
}

COMMAND_RESULT CommandGetDel::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       KeyHelper::GetDel(user, key);
       return SUCCESS;
}

CommandGetSubstr::CommandGetSubstr(Module* Creator) : Command(Creator, "SUBSTR", 3, 3)
{
         group = 'k';
         syntax = "<key> <from> <to>";
}

COMMAND_RESULT CommandGetSubstr::Handle(User* user, const Params& parameters)
{
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       int limit = convto_num<int>(parameters[2]); 
       int offset = convto_num<int>(parameters[1]);

       KeyHelper::GetSubstr(user, key, offset, limit);
       return SUCCESS;
}

CommandGetExp::CommandGetExp(Module* Creator) : Command(Creator, "GETEXP", 2, 2)
{
         groups = { 'e', 'k' };
         syntax = "<seconds> <key>";
}

COMMAND_RESULT CommandGetExp::Handle(User* user, const Params& parameters)
{  
       const std::string& seconds = parameters[0];
       const std::string& key = parameters[1];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       if (!CheckValidPos(user, parameters[0]))
       {
              return FAILED;
       }
       
       KeyHelper::GetExp(user, key, seconds);
       return SUCCESS;
}

CommandIsAlpha::CommandIsAlpha(Module* Creator) : Command(Creator, "ISALPHA", 1, 1)
{
         group = 'k';
         syntax = "<key>";
}

COMMAND_RESULT CommandIsAlpha::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       KeyHelper::Alpha(user, key);
       return SUCCESS;
}

CommandIsNum::CommandIsNum(Module* Creator) : Command(Creator, "ISNUM", 1, 1)
{
         group = 'k';
         syntax = "<key>";
}

COMMAND_RESULT CommandIsNum::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       KeyHelper::IsNum(user, key);
       return SUCCESS;
}

CommandGetPersist::CommandGetPersist(Module* Creator) : Command(Creator, "GETPERSIST", 1, 1)
{
         groups = { 'e', 'k' };
         syntax = "<key>";
}

COMMAND_RESULT CommandGetPersist::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       KeyHelper::GetPersist(user, key);
       return SUCCESS;
}

CommandWDel::CommandWDel(Module* Creator) : Command(Creator, "WDEL", 1, 1)
{
         group = 'k';
         syntax = "<%key>";
}

COMMAND_RESULT CommandWDel::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckFormat(user, key))
       {
            return FAILED;
       }

       KeyHelper::WDelete(user, key);
       return SUCCESS;
}

CommandIsBool::CommandIsBool(Module* Creator) : Command(Creator, "ISBOOL", 1, 1)
{
         group = 'k';
         syntax = "<key>";
}

COMMAND_RESULT CommandIsBool::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       KeyHelper::IsBool(user, key);
       return SUCCESS;
}

CommandAsBool::CommandAsBool(Module* Creator) : Command(Creator, "ASBOOL", 1, 1)
{
         group = 'k';
         syntax = "<key>";
}

COMMAND_RESULT CommandAsBool::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       KeyHelper::AsBool(user, key);
       return SUCCESS;
}

CommandIsMatch::CommandIsMatch(Module* Creator) : Command(Creator, "ISMATCH", 2, 2)
{
         group = 'k';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandIsMatch::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];

       if (!CheckFormat(user, value))
       {
            return FAILED;
       }

       KeyHelper::IsMatch(user, key, value);
       return SUCCESS;
}
