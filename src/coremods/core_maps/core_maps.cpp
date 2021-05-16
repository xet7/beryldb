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
#include "core_maps.h"

class CoreModuleMaps : public Module
{
    private:
    
        CommandHSET cmdhset;
        CommandHGET cmdhget;
        CommandHDel cmdhdel;
        CommandHCount cmdhcount;
        CommandHSearch cmdhsearch;
        CommandHDelAll cmdhdelall;
        CommandHSeek cmdhseek;
        CommandHKeys cmdhkeys;
        CommandHExists cmdhexists;
        CommandHMove cmdhmove;
        
    public:	
        
        CoreModuleMaps() : cmdhset(this), 
                           cmdhget(this),
                           cmdhdel(this),
                           cmdhcount(this),
                           cmdhsearch(this),
                           cmdhdelall(this),
                           cmdhseek(this),
                           cmdhkeys(this),
                           cmdhexists(this),
                           cmdhmove(this)
                           
        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides commands to handle maps.", VF_BERYLDB);
        }
};

MODULE_LOAD(CoreModuleMaps)
