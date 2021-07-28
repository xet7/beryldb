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
#include "core_maps.h"

CommandHGet::CommandHGet(Module* Creator) : Command(Creator, "HGET", 2, 2)
{
         group = 'm';
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHGet::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap     	= 	parameters[0];
       const std::string& key      	= 	parameters[1];

       if (!CheckKey(user, key) || !CheckKey(user, kmap))
       {
            return FAILED;
       }

       MapsHelper::Get(user, kmap, key);
       return SUCCESS;
}

CommandHCount::CommandHCount(Module* Creator) : Command(Creator, "HCOUNT", 1, 1)
{
         group = 'm';
         syntax = "<map>";
}

COMMAND_RESULT CommandHCount::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap = parameters[0];

        if (!CheckKey(user, kmap))
        {
             return FAILED;
        }

       MapsHelper::Count(user, kmap);
       return SUCCESS;
}

CommandHExists::CommandHExists(Module* Creator) : Command(Creator, "HEXISTS", 2, 2)
{
         group = 'm';
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHExists::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap 		= 	parameters[0];
       const std::string& key		= 	parameters[1];

       if (!CheckKey(user, key) || !CheckKey(user, kmap))
       {
            return FAILED;
       }

       MapsHelper::Exists(user, kmap, key);
       return SUCCESS;
}

CommandHStrlen::CommandHStrlen(Module* Creator) : Command(Creator, "HSTRLEN", 2, 2)
{
         group = 'm';
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHStrlen::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap 		= 	parameters[0];
       const std::string& key 		= 	parameters[1];

       if (!CheckKey(user, key) || !CheckKey(user, kmap))
       {
            return FAILED;
       }

       MapsHelper::Strlen(user, kmap, key);
       return SUCCESS;
}

CommandHVals::CommandHVals(Module* Creator) : Command(Creator, "HVALS", 1, 3)
{
         group = 'm';
         syntax = "<map> <offset> <limit>";
}

COMMAND_RESULT CommandHVals::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap = parameters[0];

       if (!CheckKey(user, kmap))
       {
            return FAILED;
       }

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);

       if (lms[0] == 0)
       {
            return FAILED; 
       }

       signed int offset = lms[1];
       signed int limit = lms[2];

       MapsHelper::Vals(user, kmap, offset, limit);
       return SUCCESS;
}

CommandHGetAll::CommandHGetAll(Module* Creator) : Command(Creator, "HGETALL", 1, 3)
{
         group = 'm';
         syntax = "<map> <offset> <limit>";
}

COMMAND_RESULT CommandHGetAll::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap = parameters[0];

       if (!CheckKey(user, kmap))
       {
            return FAILED;
       }

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);

       if (lms[0] == 0)
       {
            return FAILED; 
       }

       signed int offset = lms[1];
       signed int limit = lms[2];

       MapsHelper::GetAll(user, kmap, offset, limit);
       return SUCCESS;
}

CommandHDel::CommandHDel(Module* Creator) : Command(Creator, "HDEL", 2, 2)
{
         group = 'm';
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHDel::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap 		= 	parameters[0];
       const std::string& key 		= 	parameters[1];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       MapsHelper::Delete(user, kmap, key);
       return SUCCESS;
}

CommandHWDel::CommandHWDel(Module* Creator) : Command(Creator, "HWDEL", 2, 2)
{
         group = 'm';
         syntax = "<map> <key>";
}

COMMAND_RESULT CommandHWDel::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap 		= 	parameters[0];
       const std::string& key 		= 	parameters[1];
       
       if (!CheckKey(user, kmap))
       {
            return FAILED;
       }
       
       MapsHelper::WDelete(user, kmap, key);
       return SUCCESS;
}

CommandHSet::CommandHSet(Module* Creator) : Command(Creator, "HSET", 3, 3)
{
         group = 'm';
         syntax = "<map> <key> \"value\"";
}

COMMAND_RESULT CommandHSet::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap  = parameters[0];
       const std::string& key   = parameters[1];
       const std::string& value = parameters.back();

        if (!CheckKey(user, key) || !CheckKey(user, kmap))
        {
            return FAILED;
        }

       if (!CheckFormat(user, value))
       {
            return FAILED;
       }
       
       MapsHelper::Set(user, kmap, key, value);
       return SUCCESS;
}

CommandHSetNX::CommandHSetNX(Module* Creator) : Command(Creator, "HSETNX", 3, 3)
{
         group = 'm';
         syntax = "<map> <key> \"value\"";
}

COMMAND_RESULT CommandHSetNX::Handle(User* user, const Params& parameters)
{  
       const std::string& kmap  	= 	parameters[0];
       const std::string& key   	= 	parameters[1];
       const std::string& value 	= 	parameters.back();

       if (!CheckKey(user, key) || !CheckKey(user, kmap))
       {
            return FAILED;
       }

       if (!CheckFormat(user, value))
       {
            return FAILED;
       }

       MapsHelper::SetNX(user, kmap, key, value);
       return SUCCESS;
}

