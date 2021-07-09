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
#include "managers/lists.h"
#include "engine.h"
#include "core_list.h"

CommandLResize::CommandLResize(Module* Creator) : Command(Creator, "LRESIZE", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandLResize::Handle(User* user, const Params& parameters)
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
       
       ListHelper::Resize(user, key, value);
       return SUCCESS;  
}

CommandLGet::CommandLGet(Module* Creator) : Command(Creator, "LGET", 1, 3)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandLGet::Handle(User* user, const Params& parameters)
{  
       const std::string key = parameters[0];
        
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
         syntax = "<key> <%value> <offset> <limit>";
}

COMMAND_RESULT CommandLFind::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];
        
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
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandLPos::Handle(User* user, const Params& parameters)
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
       
       ListHelper::Index(user, key, value);
       return SUCCESS;  
}

CommandLRepeats::CommandLRepeats(Module* Creator) : Command(Creator, "LREPEATS", 2, 2)
{
         syntax = "<key> <value>";
}

COMMAND_RESULT CommandLRepeats::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       const std::string& value = parameters[1];

        if (!Daemon::CheckFormat(user, value))
        {
            return FAILED;
        }

        ListHelper::Repeats(user, key, value);
        return SUCCESS;  
}

CommandLRop::CommandLRop(Module* Creator) : Command(Creator, "LROP", 1, 1)
{
         syntax = "<key>";
}

COMMAND_RESULT CommandLRop::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       ListHelper::Rop(user, key);
       return SUCCESS;  
}

