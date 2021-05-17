/*
 * BerylDB - A modular database.
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
#include "brldb/hquery.h"
#include "core_hquery.h"
#include "managers/maps.h"

CommandHLimits::CommandHLimits(Module* parent) : Command(parent, "HLIMITS", 2, 2) 
{
        syntax = "<limit> <offset>";
}

COMMAND_RESULT CommandHLimits::Handle(User* user, const Params& parameters)
{
        if (user->hquery == nullptr)
        {
                user->SendProtocol(ERR_NO_HQUERY, "No hquery found");
                return FAILED;
        }

        const std::string& offset = parameters[0];
        const std::string& limit = parameters[1];
        
        if (!is_zero_or_great(offset) || !is_zero_or_great(limit))
        {
                user->SendProtocol(ERR_USE, ERR_GREAT_ZERO, MUST_BE_GREAT_ZERO.c_str());
                return FAILED;
        }
        
        user->hquery->offset = convto_num<unsigned int>(offset);
        user->hquery->limit = convto_num<unsigned int>(limit);
        
        user->SendProtocol(BRLD_HLIMITS_OK, offset, limit, "HLimits set.");
        return SUCCESS;
}

CommandHSend::CommandHSend(Module* parent) : Command(parent, "HSEND", 0) 
{

}

COMMAND_RESULT CommandHSend::Handle(User* user, const Params& parameters)
{
        if (user->hquery == nullptr)
        {
                user->SendProtocol(ERR_NO_HQUERY, "No hquery found");
                return FAILED;
        }
        
        user->SendProtocol(BRLD_HQUERY_SENT, "HQuery sent.");
        MapsHelper::PreHQuery(user, user->current_db, user->select, user->hquery);
        return SUCCESS;
}

CommandHParam::CommandHParam(Module* parent) : Command(parent, "HPARAM", 2, 2) 
{
        syntax = "<var> <param>";
}

COMMAND_RESULT CommandHParam::Handle(User* user, const Params& parameters)
{
        if (user->hquery == nullptr)
        {
                user->SendProtocol(ERR_NO_HQUERY, "No hquery found");
                return FAILED;
        }

        const std::string& var = parameters[0];
        const std::string& param = parameters.back();
        
        if (!Daemon::CheckFormat(user, param))
        {
             return FAILED;
        }
        
        const unsigned int i_var = convto_num<unsigned int>(var);
        
        switch (i_var)
        {
                case 1:
                
                     {
                          user->hquery->field = param;       
                          break;
                     }
                     
                case 2:
                
                     {
                          user->hquery->sort = param;       
                          break;
                     }
                     
                case 3:
                
                     {
                          user->hquery->key = param;       
                          break;
                     }
                     
                
                default:
                
                     { 
                          user->SendProtocol(ERR_WRONG_HPARAMS, "Wrong param.");
                          return FAILED;     
                     }
                
        }
        
        user->SendProtocol(BRLD_HPARAM_OK, PROCESS_OK);
        return SUCCESS;
}