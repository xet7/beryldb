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
#include "maker.h"
#include "managers/vectors.h"
#include "engine.h"
#include "core_vector.h"

CommandVExist::CommandVExist(Module* Creator) : Command(Creator, "VEXISTS", 2, 2)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVExist::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];

       if (!CheckFormat(user, value))
       {
           return FAILED;
       }
       
       VectorHelper::Exist(user, key, value);
       return SUCCESS;  
}

CommandVPos::CommandVPos(Module* Creator) : Command(Creator, "VPOS", 2, 2)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVPos::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];

       if (!is_number(value))
       {
                 user->SendProtocol(ERR_QUERY, MUST_BE_NUMERIC);
                 return FAILED;
       }

       if (!is_positive_number(value))
       {
                user->SendProtocol(ERR_QUERY, MUST_BE_POSIT);
                return FAILED;
       }
       
       VectorHelper::Index(user, key, value);
       return SUCCESS;  
}

CommandVGet::CommandVGet(Module* Creator) : Command(Creator, "VGET", 1, 3)
{
         group = 'v';
         syntax = "<key> <offset> <limit>";
}

COMMAND_RESULT CommandVGet::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);
       
       if (lms[0] == 0)
       {
            return FAILED; 
       }
       
       signed int offset = lms[1];
       signed int limit = lms[2];

       VectorHelper::Get(user, key, offset, limit);
       return SUCCESS;  
}

CommandVCount::CommandVCount(Module* Creator) : Command(Creator, "VCOUNT", 1, 1)
{
         group = 'v';
         syntax = "<key>";
}

COMMAND_RESULT CommandVCount::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       VectorHelper::Count(user, key);
       return SUCCESS;  
}

CommandVResize::CommandVResize(Module* Creator) : Command(Creator, "VRESIZE", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVResize::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];
       
       if (!is_number(value))
       {
                 user->SendProtocol(ERR_USE, DBL_NOT_NUM, MUST_BE_NUMERIC.c_str());
                 return FAILED;
       }

       if (!is_positive_number(value))
       {
                user->SendProtocol(ERR_USE, ERR_MUST_BE_POS_INT, MUST_BE_POSIT.c_str());
                return FAILED;
       }

       VectorHelper::Resize(user, key, value);
       return SUCCESS;  
}

CommandVKeys::CommandVKeys(Module* Creator) : Command(Creator, "VKEYS", 1, 3)
{
         group = 'v';
         syntax = "<%key> <offset> <limit>";
}

COMMAND_RESULT CommandVKeys::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);
       
       if (lms[0] == 0)
       {
            return FAILED; 
       }
       
       signed int offset = lms[1];
       signed int limit = lms[2];

       VectorHelper::Keys(user, key, offset, limit);
       return SUCCESS;  
}

CommandVDel::CommandVDel(Module* Creator) : Command(Creator, "VDEL", 2, 2)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVDel::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters.back();
        
        if (!CheckFormat(user, value))
        {
             return FAILED;
        }

        VectorHelper::Del(user, key, value);
        return SUCCESS;  
}

CommandVEraseFrom::CommandVEraseFrom(Module* Creator) : Command(Creator, "VERASEFROM", 2, 2)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVEraseFrom::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        const std::string& value = parameters.back();

        if (!CheckFormat(user, value))
        {
             return FAILED;
        }
        
        if (!is_number(value, true))
        {
                user->SendProtocol(ERR_QUERY, MUST_BE_NUMERIC);
                return FAILED;
        }

        VectorHelper::EraseFrom(user, key, value);
        return SUCCESS;
}

CommandVReverse::CommandVReverse(Module* Creator) : Command(Creator, "VREVERSE", 1, 1)
{
         group = 'v';
         syntax = "<key>";
}

COMMAND_RESULT CommandVReverse::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];
        
        VectorHelper::Reverse(user, key);
        return SUCCESS;  
}

CommandVRepeats::CommandVRepeats(Module* Creator) : Command(Creator, "VREPEATS", 2, 2)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVRepeats::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];

        if (!CheckFormat(user, value))
        {
            return FAILED;
        }

        VectorHelper::Repeats(user, key, value);
        return SUCCESS;  
}

CommandVSort::CommandVSort(Module* Creator) : Command(Creator, "VSORT", 1, 1)
{
         group = 'v';
         syntax = "<key>";
}

COMMAND_RESULT CommandVSort::Handle(User* user, const Params& parameters)
{  
        const std::string& key = parameters[0];

        VectorHelper::Sort(user, key);
        return SUCCESS;  
}
