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

class CommandSetAgent : public Command
{
   public:
   
        CommandSetAgent(Module* Creator) : Command(Creator, "SETAGENT", 2)
        {
                last_empty_ok = false;
                requires = 'e';
                syntax = "[<instance>] <channel>[,<channel>]+";
                INTERPRET2(TR_INSTANCE, TR_TEXT);
        }
        
        COMMAND_RESULT Handle(User* user, const Params& parameters)
        {
                const std::string& instance = parameters[0];
                const std::string& agent = parameters[1];
                
                User* target = Kernel->Clients->FindInstance(instance);

                if ((!target) || (target->registered != REG_OK))
                {
                        user->SendProtocol(Protocols::NoInstance(instance));
                        return FAILED;
                }
                
                if (agent.empty() || agent.length() < 3 || agent.length() > 15)
                {
                        user->SendProtocol(ERR_INPUT2, ERR_BAD_FORMAT, VALID_AGENT);
                        return FAILED;
                }

                if (!Kernel->Engine->IsAgent(agent))
                {
                        user->SendProtocol(ERR_INPUT2, ERR_BAD_FORMAT, WRONG_AGENT);
                        return FAILED;
                }
                
                if (IS_LOCAL(target))
                {
                        target->SetAgent(parameters[1]);
                        user->SendProtocol(BRLD_INPUT, PROCESS_OK);
                }                
                
                return SUCCESS;
        }
        
        RouteParams GetRouting(User* user, const Params& parameters)
        {
                return ROUTE_OPT_UCAST(parameters[0]);
        }
        
};

class ModuleSetAgent : public Module
{
  private:
  
        CommandSetAgent cmd;

 public:

        ModuleSetAgent() : cmd(this)
        {
        
        }

        Version GetDescription()
        {
                return Version("Adds SETAGENT command.", VF_BERYLDB|VF_OPTCOMMON);
        }
};

MODULE_LOAD(ModuleSetAgent)
