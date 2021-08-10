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

#pragma once

#include "beryl.h"

class ServerTargetCommand : public Command
{
 public:

        ServerTargetCommand(Module* mod, const std::string& Name) : Command(mod, Name)
        {
        
        }

        RouteParams GetRouting(User* user, const Params& parameters) ;
};

/* 
 * Retrieves a configuration setting.
 * 
 * @requires 'm'.
 *
 * @parameters:
 *
 *	   · string	 : setting.
 *
 * @protocol:
 *
 *         · enum        : NULL, INVALID_PARAM.
 *         · string      : value.
 */

class CommandGetConfig : public ServerTargetCommand
{
    public:
    
     CommandGetConfig(Module* Creator);
     
     COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Retrieves a configuration setting.
 * 
 * @requires 'm'.
 *
 * @parameters:
 *
 *         · string      : setting key.
 *         · string      : setting value.
 *
 * @protocol:
 *
 *         · enum        : OK, NULL, INVALID_PARAM.
 */

class CommandSetConfig : public ServerTargetCommand
{
    public:
    
     CommandSetConfig(Module* Creator);
     
     COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Resets all configuration settings to default.
 * 
 * @requires 'm'.
 *
 *         · enum        : OK.
 */

class CommandDefaults : public ServerTargetCommand
{
    public:
    
     CommandDefaults(Module* Creator);
     
     COMMAND_RESULT Handle(User* user, const Params& parameters);
};


