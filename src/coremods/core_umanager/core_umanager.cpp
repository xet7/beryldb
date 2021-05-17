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
#include "core_umanager.h"

class CoreModUManager : public Module
{
  private:
  
        CommandAddUser cmdauser;
        CommandSetFlags cmdsetflags;
        CommandDelFlags cmddelflags;
        CommandFlags cmdflags;
        CommandListUsers CommandListusers;
        CommandChangePass cmdchangepass;
        CommandDelUser cmddeluser;
        CommandListAdmins CommandListadmins;
        CommandInCache cmdincache;
        CommandResetCache cmdresetcache;
        
  public:
  
        CoreModUManager() : cmdauser(this)
                            , cmdsetflags(this)
                            , cmddelflags(this)
                            , cmdflags(this)
                            , CommandListusers(this)
                            , cmdchangepass(this)
                            , cmddeluser(this)
                            , CommandListadmins(this)
                            , cmdincache(this)
                            , cmdresetcache(this)
        {

        }

        Version GetDescription() 
        {
                return Version("Provides management-related functions.", VF_BERYLDB | VF_CORE);
        }
};

MODULE_LOAD(CoreModUManager)

        
        
