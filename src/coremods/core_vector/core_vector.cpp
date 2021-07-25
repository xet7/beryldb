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
#include "core_vector.h"

class CoreModuleVector : public Module
{
    private:
    
        CommandVPush cmdvpush;
        CommandVExist cmdvexist;
        CommandVCount cmdvcount;
        CommandVPos cmdvpos;
        CommandVGet cmdvget;
        CommandVResize cmdvresize;
        CommandVKeys cmdvkeys;
        CommandVPopBack cmdvpopback;
        CommandVPopFront cmdvpopfront;
        CommandVDel cmdvdel;
        CommandVEraseFrom cmdverasefrom;
        CommandVReverse cmdvreverse;
        CommandVRepeats cmdvrepeats;
        CommandVSort cmdvsort;
        CommandVAvg cmdavg;
        CommandVHigh cmdvhigh;
        CommandVLow cmdvlow;
        CommandVSum cmdvsum;
        CommandVPushNX cmdvpushnx;
        
    public:	
        
        CoreModuleVector() : cmdvpush(this),
                             cmdvexist(this),
                             cmdvcount(this),
                             cmdvpos(this),
                             cmdvget(this),
                             cmdvresize(this),
                             cmdvkeys(this),
                             cmdvpopback(this),
                             cmdvpopfront(this),
                             cmdvdel(this),
                             cmdverasefrom(this),
                             cmdvreverse(this),
                             cmdvrepeats(this),
                             cmdvsort(this),
                             cmdavg(this),
                             cmdvhigh(this),
                             cmdvlow(this),
                             cmdvsum(this),
                             cmdvpushnx(this)
                             

        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides vector-related commands.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModuleVector)
