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
#include "core_expires.h"

class CoreModuleExpires : public Module
{
    private:
    
        CommandExpire      cmdexpire;
        CommandTTL         cmdttl;
        CommandSetex       cmdsetex;
        CommandPersist     cmdpersist;
        CommandExpireCount cmdexpcount;
        CommandReset       cmdreset;
        CommandExpireAT    cmdexpireat;
        CommandSReset	   cmdsreset;
        
    public:     
        
        CoreModuleExpires() : cmdexpire(this), cmdttl(this), cmdsetex(this), cmdpersist(this),
                           cmdexpcount(this), cmdreset(this), cmdexpireat(this), cmdsreset(this)
        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides expire and associated commands.", VF_BERYLDB);
        }
};

MODULE_LOAD(CoreModuleExpires)

