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
#include "core_admin.h"

class CoreModuleAdmin : public Module
{
  private:
  
	CommandLogout cmdlogout;
        CommandFinger cmdfinger;	
        CommandLogins cmdlogins;
        CommandShutdown cmdshutdown;
        CommandRestart cmdrestart;
        
  public:
  
	CoreModuleAdmin() : cmdlogout(this), cmdfinger(this), cmdlogins(this), cmdshutdown(this), cmdrestart(this)
	{

	}

	Version GetDescription() 
	{
		return Version("Provides LOGOUT, LOGINS, and FINGER commands.", VF_BERYLDB | VF_CORE);
	}
};

MODULE_LOAD(CoreModuleAdmin)
