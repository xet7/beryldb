/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
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

CommandHQuery::CommandHQuery(Module* parent) : Command(parent, "HQUERY", 0) 
{

}

COMMAND_RESULT CommandHQuery::Handle(User* user, const Params& parameters)
{
        if (user->hquery == nullptr)
        {
                user->SendProtocol(BRLD_PREPARE_OK, "Preparing");
        }
        else
        {
                user->SendProtocol(BRLD_HOVER_OK, "Overriding prepare.");
                user->hquery.reset();
        }
        
        std::shared_ptr<HQuery> newquery = std::make_shared<HQuery>();
        user->hquery = newquery;
        return SUCCESS;
}

CommandHReset::CommandHReset(Module* parent) : Command(parent, "HRESET", 0) 
{

}

COMMAND_RESULT CommandHReset::Handle(User* user, const Params& parameters)
{
        if (user->hquery != nullptr)
        {
                user->SendProtocol(BRLD_HOVER_OK, "Reseting");
                user->hquery.reset();
                user->hquery = nullptr;
        }
        
        user->SendProtocol(ERR_HUNABLE_RESET, "Not found");
        
        return SUCCESS;
}
