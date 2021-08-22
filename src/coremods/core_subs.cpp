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
#include "engine.h"
#include "channelmanager.h"

class CommandSubs : public Command
{
    public:
      
      CommandSubs(Module* parent) : Command(parent, "LISTCHANS", 0, 0)
      {
            last_empty_ok = false;
      }

      COMMAND_RESULT Handle(User* user, const Params& parameters);
};

COMMAND_RESULT CommandSubs::Handle(User* user, const Params& parameters)
{
        size_t minusers = 0;
        size_t maxusers = 0;

        for (Params::const_iterator iter = parameters.begin(); iter != parameters.end(); ++iter)
        {
                const std::string& constraint = *iter;

                if (constraint[0] == '<')
                {
                        maxusers = convto_num<size_t>(constraint.c_str() + 1);
                }
                else if (constraint[0] == '>')
                {
                        minusers = convto_num<size_t>(constraint.c_str() + 1);
                }
       }
       
      const ChanMap& chans = Kernel->Channels->GetSubs();
      
      unsigned int counter = 0;

      Dispatcher::JustAPI(user, BRLD_START_LIST);

      Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s | %-10s", "Name", "Clients"));
      Dispatcher::JustEmerald(user, BRLD_START_LIST, Daemon::Format("%-30s | %-10s", Dispatcher::Repeat("―", 30).c_str(), Dispatcher::Repeat("―", 10).c_str()));
       
      for (ChanMap::const_iterator i = chans.begin(); i != chans.end(); ++i)
      {
                     Channel* const chan = i->second;
                     const size_t users = chan->GetClientCounter();
                     
                     if ((minusers && users <= minusers) || (maxusers && users >= maxusers))
                     {
                          continue;
                     }
                     
                     counter++;
                     Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s | %-10s", chan->GetName().c_str(), convto_string(users).c_str()), Daemon::Format("%s %s", chan->GetName().c_str(), convto_string(users).c_str()));
      }

      Dispatcher::JustAPI(user, BRLD_END_LIST);
            
      return SUCCESS;
};

class ModuleCoreSubs : public Module
{
 private:

        CommandSubs cmd;

 public:

        ModuleCoreSubs() : cmd(this)
        {
        
        }

        Version GetDescription()
        {
                return Version("Provides subs command.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleCoreSubs)
