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
#include "core_list.h"

CommandLResize::CommandLResize(Module* Creator) : Command(Creator, "LRESIZE", 2, 2)
{
         group = 'l';
         syntax = "<key> <size>";
}

COMMAND_RESULT CommandLResize::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       if (!CheckValidPos(user, value))
       {
              return FAILED;
       }
       
       ListHelper::Resize(user, key, value);
       return SUCCESS;  
}

CommandLGet::CommandLGet(Module* Creator) : Command(Creator, "LGET", 1, 3)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandLGet::Handle(User* user, const Params& parameters)
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

       ListHelper::Get(user, key, offset, limit);
       return SUCCESS;  
}

CommandLKeys::CommandLKeys(Module* Creator) : Command(Creator, "LKEYS", 1, 3)
{
         group = 'l';
         syntax = "<%key> <offset> <limit>";
}

COMMAND_RESULT CommandLKeys::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       std::vector<signed int> lms = GetLimits(user, this->max_params, parameters);
       
       if (lms[0] == 0)
       {
            return FAILED; 
       }
       
       signed int offset = lms[1];
       signed int limit = lms[2];

       ListHelper::Keys(user, key, offset, limit);
       return SUCCESS;  
}

CommandLFind::CommandLFind(Module* Creator) : Command(Creator, "LFIND", 2, 4)
{
         group = 'l';
         syntax = "<key> <%value> <offset> <limit>";
}

COMMAND_RESULT CommandLFind::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters.back();

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

       ListHelper::Find(user, key, value, offset, limit);
       return SUCCESS;  
}

CommandLPos::CommandLPos(Module* Creator) : Command(Creator, "LPOS", 2, 2)
{
         group = 'l';
         syntax = "<key> \"value\"";
}

COMMAND_RESULT CommandLPos::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters.back();

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       if (!CheckValidPos(user, value))
       {
              return FAILED;
       }
       
       ListHelper::Index(user, key, value);
       return SUCCESS;  
}

CommandLRepeats::CommandLRepeats(Module* Creator) : Command(Creator, "LREPEATS", 2, 2)
{
         group = 'l';
         syntax = "<key> \"value\"";
}

COMMAND_RESULT CommandLRepeats::Handle(User* user, const Params& parameters)
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

       ListHelper::Repeats(user, key, value);
       return SUCCESS;  
}

CommandLRop::CommandLRop(Module* Creator) : Command(Creator, "LRPOP", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandLRop::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       ListHelper::Rop(user, key);
       return SUCCESS;  
}

CommandFRop::CommandFRop(Module* Creator) : Command(Creator, "LFPOP", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandFRop::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       ListHelper::FRop(user, key);
       return SUCCESS;  
}

CommandLPush::CommandLPush(Module* Creator) : Command(Creator, "LPUSH", 2, 2)
{
         group = 'l';
         syntax = "<key> \"value\"";
}

COMMAND_RESULT CommandLPush::Handle(User* user, const Params& parameters)
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

        ListHelper::Push(user, key, value);
        return SUCCESS;  
}

CommandLExist::CommandLExist(Module* Creator) : Command(Creator, "LEXISTS", 2, 2)
{
         group = 'l';
         syntax = "<key> \"value\"";
}

COMMAND_RESULT CommandLExist::Handle(User* user, const Params& parameters)
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
       
       ListHelper::Exist(user, key, value);
       return SUCCESS;  
}

CommandLCount::CommandLCount(Module* Creator) : Command(Creator, "LCOUNT", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandLCount::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       ListHelper::Count(user, key);
       return SUCCESS;  
}

CommandLBack::CommandLBack(Module* Creator) : Command(Creator, "LBACK", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandLBack::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }
       
       ListHelper::Rop(user, key);
       return SUCCESS;  
}

CommandLFront::CommandLFront(Module* Creator) : Command(Creator, "LFRONT", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandLFront::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       ListHelper::Front(user, key);
       return SUCCESS;  
}

CommandLPushNX::CommandLPushNX(Module* Creator) : Command(Creator, "LPUSHNX", 2, 2)
{
         group = 'v';
         syntax = "<key> \"value\"";
}

COMMAND_RESULT CommandLPushNX::Handle(User* user, const Params& parameters)
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

        ListHelper::PushNX(user, key, value);
        return SUCCESS;  
}

CommandLAvg::CommandLAvg(Module* Creator) : Command(Creator, "LAVG", 1, 1)
{
         group = 'v';
         syntax = "<key>";
}

COMMAND_RESULT CommandLAvg::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       ListHelper::Avg(user, key);
       return SUCCESS;  
}

