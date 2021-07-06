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
#include "core_keys.h"

CommandGet::CommandGet(Module* Creator) : Command(Creator, "GET", 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandGet::Handle(User* user, const Params& parameters)
{  
       std::string key = parameters[0];
       
       /* 
        * The false at the end indicates that we are
        * not requesting for a strlen. 
        */

       KeyHelper::Get(user, key);
       return SUCCESS;
}


CommandStrlen::CommandStrlen(Module* Creator) : Command(Creator, "STRLEN", 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandStrlen::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       /* The 'true' parameter at the end indicates that we are performing an strlen query. */
       
       KeyHelper::Strlen(user, key);
       return SUCCESS;
}

CommandGetDel::CommandGetDel(Module* Creator) : Command(Creator, "GETDEL", 1, 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandGetDel::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       KeyHelper::GetDel(user, key);
       return SUCCESS;
}

CommandGetSubstr::CommandGetSubstr(Module* Creator) : Command(Creator, "SUBSTR", 3, 3)
{
         syntax = "<key> <from> <to>";
}

COMMAND_RESULT CommandGetSubstr::Handle(User* user, const Params& parameters)
{
       const std::string& key = parameters[0];

       int limit = convto_num<int>(parameters[2]); 
       int offset = convto_num<int>(parameters[1]);

       KeyHelper::GetSubstr(user, key, offset, limit);
       return SUCCESS;
}

