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
#include "core_umanager.h"
#include "managers/user.h"
#include "managers/maps.h"
#include "brldb/database.h"

CommandResetCache::CommandResetCache(Module* parent) : Command(parent, "RESETCACHE", 0)
{
         requires = 'r';
}

COMMAND_RESULT CommandResetCache::Handle(User* user, const Params& parameters)
{
        Kernel->Logins->Reset();
        
        user->SendProtocol(BRLD_RESET_CACHE, PROCESS_OK);
        return SUCCESS;
}

CommandInCache::CommandInCache(Module* parent) : Command(parent, "INCACHE", 1, 1)
{
         requires = 'r';
         syntax = "<login>";
}

COMMAND_RESULT CommandInCache::Handle(User* user, const Params& parameters)
{
        const std::string& login = parameters[0];
        
        user->SendProtocol(BRLD_IN_CACHE, login, convto_string(Kernel->Logins->InCache(login)));
        return SUCCESS;
}

