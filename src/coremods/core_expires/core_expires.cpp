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
#include "core_expires.h"

class CoreModuleExpires : public Module
{
    private:
    
        CommandExpire      cmdexpire;
        CommandTTL         cmdttl;
        CommandSetex       cmdsetex;
        CommandPersist     cmdpersist;
        CommandExpireLIST cmdexplist;
        CommandReset       cmdreset;
        CommandExpireAT    cmdexpireat;
        CommandSelectReset cmdsreset;
        CommandSelectCount cmdselectcount;
        CommandDBEReset cmddbreset;
        
    public:     
        
        CoreModuleExpires() : cmdexpire(this), cmdttl(this), cmdsetex(this), cmdpersist(this),
                           cmdexplist(this), cmdreset(this), cmdexpireat(this), cmdsreset(this),
                           cmdselectcount(this), cmddbreset(this)
        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides expire and associated commands.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModuleExpires)

