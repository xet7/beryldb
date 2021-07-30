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

class CommandMassJoin : public Command
{
  public:
  
        CommandMassJoin(Module* Creator) : Command(Creator, "MASSJOIN", 1, 1)
        {
             requires = 'm';
             syntax = "<chan>";
        }

        COMMAND_RESULT Handle(User* user, const Params& parameters)
        {
             const std::string& channel = parameters[0];
             unsigned int total = 0;
             
             const UserMap& users = Kernel->Clients->GetInstances();
             
             for (UserMap::const_iterator u = users.begin(); u != users.end(); ++u)
             {
                  User* dest = u->second;

                  if (dest == NULL || dest->IsQuitting())
                  {
                        continue;
                  }
                  
                  LocalUser* localuser = IS_LOCAL(dest);
                  
                  Channel* chan = NULL;
                              
                  if (localuser)
                  {
                        chan = Channel::JoinUser(false, localuser, channel, true);
                        
                        if (chan)
                        {
                               localuser->SendProtocol(BRLD_MASS_JOIN, "MASSJOIN:" + channel);
                               total++;
                        }
                  }
             }
             
             user->SendProtocol(BRLD_OK, convto_string(total));
             return SUCCESS;

        }
        
};

class ModuleMassJoin : public Module
{
  private:
  
        CommandMassJoin cmd;

 public:
 
        ModuleMassJoin() : cmd(this)
        {
        
        }

        Version GetDescription()
        {
                return Version("Provides MASSJOIN command.", VF_BERYLDB|VF_OPTCOMMON);
        }
};

MODULE_LOAD(ModuleMassJoin)
        
