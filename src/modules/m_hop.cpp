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
#include "channelmanager.h"

class CommandHop : public MultiCommand
{
  public:
  
        CommandHop(Module* Creator) : MultiCommand(Creator, "HOP", 1)
        {
                syntax = "<channel>";
        }
        
        COMMAND_RESULT HandleLocal(LocalUser* user, const Params& parameters)
        {
                if (CommandHandler::HasLoop(user, this, parameters, 0))
                {
                        return SUCCESS;
                }

                Channel* channel = Kernel->Channels->Find(parameters[0]);

                if (!channel)
                {
                        user->SendProtocol(Protocols::NoChannel(parameters[0]));
                        return FAILED;
                }

                if (channel->IsSubscribed(user))
                {
                        channel->PartUser(user);
                        Channel::JoinUser(false, user, parameters[0], true);
                        return SUCCESS;
                }
                else
                {
                        user->SendProtocol(ERR_INPUT, NOT_SUBSCRIBED);
                }
                
                return FAILED;
        }
};

class ModuleHop : public Module
{
  private:
  
        CommandHop  cmd;

  public:
 
        ModuleHop() : cmd(this)
        {
        
        }

        Version GetDescription()
        {
                return Version("Provides HOP command.", VF_BERYLDB|VF_OPTCOMMON);
        }
};

MODULE_LOAD(ModuleHop)
        
