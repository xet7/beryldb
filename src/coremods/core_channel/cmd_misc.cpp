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
#include "core_channel.h"

CommandMyChans::CommandMyChans(Module* parent) : Command(parent, "MYCHANS", 0, 0)
{

}

COMMAND_RESULT CommandMyChans::Handle(User* user, const Params& parameters)
{
           Dispatcher::JustAPI(user, BRLD_SUBS_LIST_BEGIN);

           Dispatcher::JustEmerald(user, BRLD_SUBS_LIST_BEGIN, Daemon::Format("%-30s", "Channel"));
           Dispatcher::JustEmerald(user, BRLD_SUBS_LIST_BEGIN, Daemon::Format("%-30s", Dispatcher::Repeat("―", 30).c_str()));

           for (User::SubsList::iterator i = user->chans.begin(); i != user->chans.end(); i++)
           {
                    Channel* chan = (*i)->chan;
                    Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s", chan->GetName().c_str()), Daemon::Format("%s", chan->GetName().c_str()));
           }

           Dispatcher::JustAPI(user, BRLD_SUBS_LIST_END);
           return SUCCESS;
}

CommandChans::CommandChans(Module* parent) : Command(parent, "GETCHANS", 1, 1)
{
          syntax = "<instance>";
          flags = 'e';
}

COMMAND_RESULT CommandChans::Handle(User* user, const Params& parameters)
{
           const std::string& dest = parameters[0];
           
           User* found =  Kernel->Clients->FindInstanceOnly(dest);
           
           if (!found)
           {
                  user->SendProtocol(ERR_INPUT, PROCESS_NULL);
                  return FAILED;
           }
           
           Dispatcher::JustAPI(user, BRLD_SUBS_LIST_BEGIN);

           Dispatcher::JustEmerald(user, BRLD_SUBS_LIST_BEGIN, Daemon::Format("%-30s", "Channel"));
           Dispatcher::JustEmerald(user, BRLD_SUBS_LIST_BEGIN, Daemon::Format("%-30s", Dispatcher::Repeat("―", 30).c_str()));
           
           for (User::SubsList::iterator i = found->chans.begin(); i != found->chans.end(); i++)
           {
                    Channel* chan = (*i)->chan;
                    Dispatcher::ListDepend(user, BRLD_ITEM_LIST, Daemon::Format("%-30s", chan->GetName().c_str()), Daemon::Format("%s", chan->GetName().c_str()));
           }

           Dispatcher::JustAPI(user, BRLD_SUBS_LIST_END);
           return SUCCESS;
}
