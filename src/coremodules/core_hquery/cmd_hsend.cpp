/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"
#include "hquery.h"
#include "core_hquery.h"
#include "managers/maps.h"

/*
CommandHLimits::CommandHLimits(Module* parent) : Command(parent, "HLIMITS", 2, 2) 
{
        syntax = "<limit> <offset>"
}

COMMAND_RESULT CommandHLimits::Handle(User* user, const Params& parameters)
{
        if (user->hquery == nullptr)
        {
                user->SendProtocol(ERR_NO_HQUERY, "No hquery found");
                return FAILED;
        }
        
        user->SendProtocol(BRLD_HQUERY_SENT, "HQuery sent.");
        return SUCCESS;
}
*/
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