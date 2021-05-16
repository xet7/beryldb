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
#include "core_channel.h"

class CoreModChannel : public Module
{
   private:
   
	CommandJoin cmdjoin;
	CommandSlist cmdslist;
        CommandPart cmdpart;

  public:

	CoreModChannel() : cmdjoin(this), cmdslist(this), cmdpart(this)
	
	{

	}

	void OnInstaceReady(std::map<std::string, std::string>& tokens) 
	{
		
	}

	void OnPostJoin(Subscription* memb) 
	{
		Channel* const chan = memb->chan;
		LocalUser* const localuser = IS_LOCAL(memb->user);
	
		if (localuser->CanPerform('e'))
		{
			cmdslist.SendSlist(localuser, chan);
		}
	}

	void Prioritize() 
	{
		Kernel->Modules.SetPriority(this, I_OnPostJoin, PRIORITY_FIRST);
	}

	Version GetDescription() 
	{
		return Version("Provides JOIN and SLIST commands.", VF_BERYLDB|VF_CORE);
	}
};

MODULE_LOAD(CoreModChannel)
