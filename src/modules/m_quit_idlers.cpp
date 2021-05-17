/*
 * BerylDB - A modular database.
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

inline unsigned long GetIdle(LocalUser* usr)
{
	return Kernel->Now() - usr->touchbase;
}

class ModuleQuitIdlers : public Module
{
  private:
  
          unsigned int limit;
          
  public:
 
        void OnEveryTwoSeconds(time_t current)
        {
		const ClientManager::LocalList& users = Kernel->Clients->GetLocals();
		
		for (ClientManager::LocalList::const_iterator it = users.begin(); it != users.end();)
		{
			LocalUser* usr = *it;
			++it;
                        
			if (GetIdle(usr) >= limit)
			{
			      Kernel->Clients->Disconnect(usr, "You are idling.");
			}		  
		}        
        }
        
        void ConfigReading(config_status& status)
        {
              limit = Kernel->Config->GetConf("idlers")->as_uint("limit", 7200);                
        }
        
        ModuleQuitIdlers()
        {
            
        }

        Version GetDescription()
        {
                return Version("Disconnects users that are idling.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleQuitIdlers)
        
