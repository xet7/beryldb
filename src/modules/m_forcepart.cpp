/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"

class CommandForcePart : public Command
{
 public:
       
        CommandForcePart(Module* Creator) : Command(Creator, "FORCEPART", 2, 2)
        {
                requires = 'e';
                syntax = "<instance> <channel>[,<channel>]";
                INTERPRET3(TR_INSTANCE, TR_TEXT, TR_TEXT);
        }
        
        COMMAND_RESULT Handle(User* user, const Params& parameters)
        {
                if (CommandHandler::HasLoop(user, this, parameters, 1))
                {
                        return FAILED;
                }
                
                User* dest = Kernel->Clients->FindInstance(parameters[0]);
                Channel* channel = Kernel->Channels->Find(parameters[1]);
                
                if (!channel)
                {
                         user->SendRemoteProtocol(ERR_NO_CHAN, parameters[1], "Channel " + parameters[1] + " does not exist.");
                         return FAILED;
                }
                
                if ((dest) && (dest->registered == REG_OK) && (channel))
                {
                        if (!channel->IsSubscribed(dest))
                        {
                                user->SendRemoteProtocol(BRLD_ALERT, dest->instance + " is not subscribed to " + channel->name);
                                return FAILED;
                        }
        
                        if (IS_LOCAL(dest))
                        {
                                channel->PartUser(dest);
                        }
                
                        return SUCCESS;
                }
                else
                {
                        user->SendProtocol(ERR_INVALID_PARAM, parameters[0], parameters[1], "Instance not located.");
                }
                
                return FAILED;
        }
};

class ModuleForcePart : public Module
{ 
  private:
  
        CommandForcePart cmd;
        
 public:
 
        ModuleForcePart() : cmd(this)
        {
        
        }

        Version GetDescription()
        {
                return Version("Adds FORCEPART command.", VF_OPTCOMMON | VF_BERYLDB);
        }
};

MODULE_LOAD(ModuleForcePart)
