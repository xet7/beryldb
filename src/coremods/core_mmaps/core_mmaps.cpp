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
#include "core_mmaps.h"

class CoreModuleMMaps : public Module
{
    private:	

         CommandMSet cmdmset;
         CommandMGet cmdmget;
         CommandMCount cmdmcount;
         CommandMIter cmditer;
         CommandMDel cmdmdel;
         CommandMSeek cmdmseek;
         CommandMRepeats cmdmrepeats;
    
    public:     

        CoreModuleMMaps() : cmdmset(this), cmdmget(this), cmdmcount(this), 
                            cmditer(this), cmdmdel(this), cmdmseek(this),
                            cmdmrepeats(this)
        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides commands to handle mmaps.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModuleMMaps)
