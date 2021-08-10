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

         CommandMPush		cmdmpush;
         CommandMPushNX         cmdpushnx;
         CommandMGet 		cmdmget;
         CommandMCount 		cmdmcount;
         CommandMKeys 		cmditer;
         CommandMDel 		cmdmdel;
         CommandMSeek 		cmdmseek;
         CommandMRepeats 	cmdmrepeats;
         CommandMVals 		cmdmvals;
         CommandMGetAll 	cmdgetalls;
         CommandMIter 		cmdmfind;
    
    public:     

        CoreModuleMMaps() : cmdmpush(this),
                            cmdpushnx(this),
                            cmdmget(this), 
                            cmdmcount(this), 
                            cmditer(this), 
                            cmdmdel(this), 
                            cmdmseek(this),
                            cmdmrepeats(this), 
                            cmdmvals(this),
                            cmdgetalls(this), 
                            cmdmfind(this)
        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides commands to handle mmaps.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModuleMMaps)
