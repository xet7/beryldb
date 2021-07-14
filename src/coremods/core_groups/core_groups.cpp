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
#include "core_groups.h"

class CoreModuleGroups : public Module
{
    private:
    
        CommandGroupAdd cmdgroupadd;
        CommandGroups cmdgroups;
        CommandDelGroup cmddelgroup;
        CommandGFlags cmdgflags;
        CommandMyGroups cmdmygroups;
        CommandAssign cmdassign;
        CommandGList cmdglist;
        CommandGReset cmdgreset;
        CommandUnAssign cmddel;
        CommandGShow cmdgshow;
        CommandFlagUpdate cmdflagupdate;
        
    public:     
        
        CoreModuleGroups() : cmdgroupadd(this), cmdgroups(this), cmddelgroup(this), cmdgflags(this),
                             cmdmygroups(this), cmdassign(this), cmdglist(this),
                             cmdgreset(this), cmddel(this), cmdgshow(this),
                             cmdflagupdate(this)
        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides group-related functions.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModuleGroups)
        