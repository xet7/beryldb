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
#include "core_config.h"

CommandSetConfig::CommandSetConfig(Module* Creator) : ServerTargetCommand(Creator, "SETCONFIG")    
{
        requires = 'm';
        syntax = "[<servername>] <value> <key>";
}
     
COMMAND_RESULT CommandSetConfig::Handle(User* user, const Params& parameters)
{
        if (!parameters.size() || parameters.size() < 2)
        {
             user->SendRemoteProtocol(ERR_INVALID_PARAM, Kernel->Config->GetServerName(), this->syntax);
             return FAILED;
        }

        if (parameters.size() > 3)
        {
              user->SendRemoteProtocol(ERR_INVALID_PARAM, Kernel->Config->GetServerName(), INVALID_PARAM);
              return FAILED;
        }
        
        bool for_us = (parameters.size() == 2 || engine::equals(parameters[0], Kernel->Config->ServerName));

        if (!for_us)
        {
             return SUCCESS;
        }
        
        std::string key;
        std::string value;
        
        if (parameters.size() == 2)
        {
             key = parameters[0];
             value = parameters[1];
        }
        else if (parameters.size() > 2)
        {
             key = parameters[1];
             value = parameters[2];
        }
                
        bool result = Kernel->Config->Sets->Set(key, value, false);
        
        if (!result)
        {
             user->SendRemoteProtocol(ERR_CONFIG_NOT_FOUND, Kernel->Config->GetServerName(), SETTING_NOT_FOUND);
             return FAILED;
        }
        
        user->SendRemoteProtocol(BRLD_CONF_OK, Kernel->Config->GetServerName(), "OK");
        return SUCCESS;
}

CommandGetConfig::CommandGetConfig(Module* Creator) : ServerTargetCommand(Creator, "GETCONFIG")    
{
          syntax = "[<servername>] <setting>";
}
     
COMMAND_RESULT CommandGetConfig::Handle(User* user, const Params& parameters)
{
        if (!parameters.size())
        {
              user->SendRemoteProtocol(ERR_INVALID_PARAM, Kernel->Config->GetServerName(), this->syntax);
              return FAILED;
        }
        
        if (parameters.size() > 2)
        {
              user->SendRemoteProtocol(ERR_INVALID_PARAM, Kernel->Config->GetServerName(), INVALID_PARAM);
              return FAILED;
        }
        
        bool for_us = (parameters.size() == 1 || engine::equals(parameters[0], Kernel->Config->ServerName));

        if (!for_us)
        {
             return SUCCESS;
        }
        
        std::string key;
        
        if (parameters.size() == 1)
        {
             key = parameters[0];
        }
        else if (parameters.size() > 1)
        {
             key = parameters[1];
        }
        
        std::string value = Kernel->Config->Sets->Get(key);
        
        if (value.empty())
        {
             user->SendRemoteProtocol(ERR_CONFIG_NOT_FOUND, Kernel->Config->GetServerName(), SETTING_NOT_FOUND);
             return FAILED;
        }
        
        user->SendRemoteProtocol(BRLD_CONF_VALUE, Kernel->Config->GetServerName(), value);
        return SUCCESS;
}
