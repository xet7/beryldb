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
#include "core_config.h"

CommandDefaults::CommandDefaults(Module* Creator) : ServerTargetCommand(Creator, "DEFCONFIG")    
{
          requires = 'm';
          syntax = "[<servername>]";
}
     
COMMAND_RESULT CommandDefaults::Handle(User* user, const Params& parameters)
{
        if (parameters.size() > 1)
        {
              user->SendRemoteProtocol(ERR_INPUT3, INVALID_PARAM, Kernel->Config->GetServerName(), INVALID_PARAM);
              return FAILED;
        }
        
        bool for_us = (parameters.size() == 0 || engine::equals(parameters[0], Kernel->Config->ServerName));

        if (!for_us)
        {
             return SUCCESS;
        }
        
        Kernel->Sets->SetDefaults();
        
        user->SendRemoteProtocol(BRLD_OK2, Kernel->Config->GetServerName(), PROCESS_OK);
        return SUCCESS;
}
