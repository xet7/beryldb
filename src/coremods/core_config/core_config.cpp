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

RouteParams ServerTargetCommand::GetRouting(User* user, const Params& parameters)
{
        if ((!parameters.empty()) && (parameters[0].find('.') != std::string::npos))
        {
                return ROUTE_UNICAST(parameters[0]);
        }
        
        return ROUTE_LOCALONLY;
}


class CoreModConfig : public Module
{
 private:

        CommandGetConfig 	cmdgetconfig;
        CommandSetConfig 	cmdsetconfig;
        CommandDefaults 	cmddefaults;

 public:

        CoreModConfig() :  cmdgetconfig(this), 
                           cmdsetconfig(this), 
                           cmddefaults(this)
        {

        }

        Version GetDescription() 
        {
                return Version("Provides configuration-related commands.", VF_CORE|VF_BERYLDB);
        }
};

MODULE_LOAD(CoreModConfig)


