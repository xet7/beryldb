/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"
#include "channelmanager.h"

ChannelManager::ChannelManager()
{

}

void ChannelManager::Reset()
{
        this->ChannelList.clear();    
}

ChannelManager::~ChannelManager()
{
        this->Reset();
}

Channel* ChannelManager::Find(const std::string &chan)
{
	ChanMap::iterator it = this->ChannelList.find(chan);

	if (it == this->ChannelList.end())
	{
		return NULL;
	}

	return it->second;
}

void ChannelManager::ShouldExists(Channel* chan)
{
        if (!chan->subscribedlist.empty())
        {
                return;
        }

        ModuleResult res;
        UNTIL_RESULT(OnChannelPreDelete, res, (chan));

        if (res == MOD_RES_STOP)
        {
                return;
        }

        ChanMap::iterator iter = Kernel->Channels->ChannelList.find(chan->name);

        if ((iter == Kernel->Channels->ChannelList.end()) || (iter->second != chan))
        {
                return;
        }

        NOTIFY_MODS(OnChannelDelete, (chan));
        Kernel->Channels->ChannelList.erase(iter);
        Kernel->Reducer->Add(chan);
}
