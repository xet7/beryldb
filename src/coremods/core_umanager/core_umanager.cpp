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
#include "core_umanager.h"

class CoreModUManager : public Module
{
  private:
  
        CommandAddUser 		cmdauser;
        CommandDelUser          cmddeluser;
        CommandAddFlag 		cmdaddflag;
        CommandDelFlags 	cmddelflags;
        CommandGetFlags 	cmdgetflags;
        CommandListUsers 	CommandListusers;
        CommandPasswd 		cmdpasswd;
        CommandListAdmins 	CommandListadmins;
        CommandInCache 		cmdincache;
        CommandResetCache	cmdresetcache;
        CommandListFlags 	cmdlistflags;
        CommandSetStatus	cmdsetstatus;
        CommandListStatus	cmdliststatus;
        CommandGetStatus	cmdgetstatus;
        
  public:
  
        CoreModUManager() :   cmdauser(this)
                            , cmddeluser(this)
                            , cmdaddflag(this)
                            , cmddelflags(this)
                            , cmdgetflags(this)
                            , CommandListusers(this)
                            , cmdpasswd(this)
                            , CommandListadmins(this)
                            , cmdincache(this)
                            , cmdresetcache(this)
                            , cmdlistflags(this)
                            , cmdsetstatus(this)
                            , cmdliststatus(this)
                            , cmdgetstatus(this)
        {

        }

        Version GetDescription() 
        {
                return Version("Provides management-related functions.", VF_BERYLDB | VF_CORE);
        }
};

MODULE_LOAD(CoreModUManager)

        
        
