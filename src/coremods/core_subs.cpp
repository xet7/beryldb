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

class CommandSubs : public Command
{
    public:
      
      CommandSubs(Module* parent) : Command(parent, "SUBS", 0, 0)
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

      Dispatcher::JustAPI(user, BRLD_SUBS_LIST_BEGIN);
       
      for (ChanMap::const_iterator i = chans.begin(); i != chans.end(); ++i)
      {
                     Channel* const chan = i->second;
                     const size_t users = chan->GetClientCounter();
                     
                     if ((minusers && users <= minusers) || (maxusers && users >= maxusers))
                     {
                          continue;
                     }
                     
                     counter++;
                     Dispatcher::CondList(user, BRLD_SUBS_LIST, chan->name, convto_string(users));
      }

      Dispatcher::JustAPI(user, BRLD_SUBS_LIST_END);
            
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
                return Version("Provides SUBS command.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleCoreSubs)
