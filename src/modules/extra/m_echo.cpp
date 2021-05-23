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

class CommandEcho : public Command
{
    public:
       
        CommandEcho(Module* Creator) : Command(Creator, "ECHO", 1, 1)
        {
              syntax = "<string>";
        }
        
        COMMAND_RESULT Handle(User* user, const Params& parameters)
        {
              const std::string& msg = parameters.back();
              
              user->SendProtocol(BRLD_TEST, Daemon::Format("echo: %s", msg.c_str()));          
              return SUCCESS;
        }        
};

class ModuleEcho : public Module
{
  private:

        CommandEcho cmd;

  public:

        ModuleEcho() : cmd(this)
        {

        }
        
        void Initialize()
        {
                bprint(DONE, "Echo module loaded.");
        }

        Version GetDescription()
        {
                return Version("Provides ECHO module.", VF_BERYLDB|VF_OPTCOMMON);
        }
};

MODULE_LOAD(ModuleEcho)
