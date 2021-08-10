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
#include "core_admin.h"

class CoreModuleAdmin : public Module
{
  private:
  
	CommandLogout 		cmdlogout;
        CommandFinger 		cmdfinger;	
        CommandLogins 		cmdlogins;
        CommandShutdown 	cmdshutdown;
        CommandRestart 		cmdrestart;
        CommandFindFlags 	cmdfindflags;
        CommandPause 		cmdpause;
        CommandResume 		cmdresume;
        CommandIdle 		cmdidle;
        
  public:
  
	CoreModuleAdmin() : cmdlogout(this), 
	                    cmdfinger(this), 
                            cmdlogins(this), 
                            cmdshutdown(this), 
                            cmdrestart(this),
	                    cmdfindflags(this), 
	                    cmdpause(this), 
	                    cmdresume(this), 
	                    cmdidle(this)
	{

	}

	Version GetDescription() 
	{
		return Version("Provides admin-related commands.", VF_BERYLDB|VF_CORE);
	}
};

MODULE_LOAD(CoreModuleAdmin)
