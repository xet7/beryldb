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
#include "core_info.h"


CommandMyChans::CommandMyChans(Module* parent) : Command(parent, "MYCHANS", 0, 0)
{

}

COMMAND_RESULT CommandMyChans::Handle(User* user, const Params& parameters)
{
           std::string chans;
           
           unsigned int counter = 0;
           
           for (User::SubsList::iterator i = user->chans.begin(); i != user->chans.end(); i++)
           {
                    Channel* chan = (*i)->chan;
                    chans += chan->name;
                    
                    counter++;
                    
                    if (user->chans.size() != counter)
                    {
                             chans += " ";
                    }
           }
           
          if (chans.empty())
          {
               return FAILED;
          }
          
          user->SendProtocol(BRLD_MY_CHANS, chans, chans.c_str());
          return SUCCESS;
}


CommandChans::CommandChans(Module* parent) : Command(parent, "CHANS", 1, 1)
{
          syntax = "<instance>";
}

COMMAND_RESULT CommandChans::Handle(User* user, const Params& parameters)
{
           const std::string& dest = parameters[0];
           
           User* found =  Kernel->Clients->FindInstanceOnly(dest);
           
           if (!found)
           {
                  user->SendProtocol(ERR_NO_INSTANCE, dest, "Instance not found.");
                  return FAILED;
           }
           
           std::string chans;
           unsigned int counter = 0;
           
           for (User::SubsList::iterator i = found->chans.begin(); i != found->chans.end(); i++)
           {
                    Channel* chan = (*i)->chan;
                    chans += chan->name;
                    
                    counter++;
                    
                    if (user->chans.size() != counter)
                    {
                             chans += " ";
                    }
           }

          if (chans.empty())
          {
               return FAILED;
          }

          user->SendProtocol(BRLD_CHANS, chans, chans.c_str());
          return SUCCESS;
}

