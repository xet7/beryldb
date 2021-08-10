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
#include "core_list.h"

class CoreModuleList : public Module
{
    private:
    
        CommandLPush 		cmdlpush;
        CommandLPopFront 	cmdlpopfront;
        CommandLPopBack 	cmdlpopback;
        CommandPopAll 		cmdlpopall;
        CommandLGet 		cmdlget;
        CommandLExist 		cmdlexist;
        CommandLCount 		cmdlcount;
        CommandLPos 		cmdlpos;
        CommandLKeys 		cmdlkeys;
        CommandLFind 		cmdlfind;
        CommandLReverse 	cmdlreverse;
        CommandLResize 		cmdlresize;
        CommandLSort 		cmdlsort;
        CommandLDel 		cmdldel;
        CommandLRepeats 	cmdlrepeats;
        CommandLRop 		cmdlrop;
        CommandFRop 		cmdfrop;
        CommandLBack 		cmdlback;
        CommandLFront 		cmdlfront;
        CommandLPushNX 		cmdlpushnx;
        CommandLAvg 		cmdlavg;
        CommandLHigh 		cmdlhigh;
        CommandLLow 		cmdllow;
         
    public:	
        
        CoreModuleList() : cmdlpush(this),
                           cmdlpopfront(this),
                           cmdlpopback(this),
                           cmdlpopall(this),
                           cmdlget(this),
                           cmdlexist(this),
                           cmdlcount(this),
                           cmdlpos(this),
                           cmdlkeys(this),
                           cmdlfind(this),
                           cmdlreverse(this),
                           cmdlresize(this),
                           cmdlsort(this),
                           cmdldel(this),
                           cmdlrepeats(this),
                           cmdlrop(this),
                           cmdfrop(this),
                           cmdlback(this),
                           cmdlfront(this),
                           cmdlpushnx(this),
                           cmdlavg(this),
                           cmdlhigh(this),
                           cmdllow(this)

        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides list-related commands.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModuleList)
