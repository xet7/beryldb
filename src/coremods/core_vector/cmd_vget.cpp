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

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

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

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       if (!CheckValidPos(user, parameters[0]))
       {
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

       if (!CheckKey(user, key))
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

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       VectorHelper::Count(user, key);
       return SUCCESS;  
}

CommandVResize::CommandVResize(Module* Creator) : Command(Creator, "VRESIZE", 2, 2)
{
       group = 'v';
       syntax = "<key> <value>";
}

COMMAND_RESULT CommandVResize::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];
       
       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       if (!CheckValidPos(user, parameters[0]))
       {
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

        if (!CheckKey(user, key))
        {
             return FAILED;
        }
        
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

        if (!CheckKey(user, key))
        {
            return FAILED;
        }

        if (!CheckFormat(user, value))
        {
             return FAILED;
        }
        
        if (!is_number(value, true))
        {
                user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
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
   
        if (!CheckKey(user, key))
        {
            return FAILED;
        }
     
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

        if (!CheckKey(user, key))
        {
             return FAILED;
        }

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

        if (!CheckKey(user, key))
        {
             return FAILED;
        }

        VectorHelper::Sort(user, key);
        return SUCCESS;  
}

CommandVPush::CommandVPush(Module* Creator) : Command(Creator, "VPUSH", 2, 2)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVPush::Handle(User* user, const Params& parameters)
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

        VectorHelper::Push(user, key, value);
        return SUCCESS;  
}

CommandVPopFront::CommandVPopFront(Module* Creator) : Command(Creator, "VPOPFRONT", 1, 1)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVPopFront::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];

        if (!CheckKey(user, key))
        {
             return FAILED;
        }

        VectorHelper::PopFront(user, key);
        return SUCCESS;  
}

CommandVPopBack::CommandVPopBack(Module* Creator) : Command(Creator, "VPOPBACK", 1, 1)
{
         group = 'v';
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandVPopBack::Handle(User* user, const Params& parameters)
{  
        const std::string key = parameters[0];

        if (!CheckKey(user, key))
        {
            return FAILED;
        }

        VectorHelper::PopBack(user, key);
        return SUCCESS;  
}

CommandVAvg::CommandVAvg(Module* Creator) : Command(Creator, "VAVG", 1, 1)
{
         group = 'v';
         syntax = "<key>";
}

COMMAND_RESULT CommandVAvg::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       VectorHelper::Avg(user, key);
       return SUCCESS;  
}

CommandVHigh::CommandVHigh(Module* Creator) : Command(Creator, "VHIGH", 1, 1)
{
         group = 'v';
         syntax = "<key>";
}

COMMAND_RESULT CommandVHigh::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       VectorHelper::High(user, key);
       return SUCCESS;  
}

CommandVLow::CommandVLow(Module* Creator) : Command(Creator, "VLOW", 1, 1)
{
         group = 'v';
         syntax = "<key>";
}

COMMAND_RESULT CommandVLow::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       VectorHelper::Low(user, key);
       return SUCCESS;  
}

CommandVSum::CommandVSum(Module* Creator) : Command(Creator, "VSUM", 1, 1)
{
         group = 'v';
         syntax = "<key>";
}

COMMAND_RESULT CommandVSum::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       VectorHelper::Sum(user, key);
       return SUCCESS;  
}

