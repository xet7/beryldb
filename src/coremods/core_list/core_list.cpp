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
    
        CommandLPush cmdlpush;
        CommandLPOP cmdlpop;
        CommandL_POP_ALL cmdlpopall;
        CommandLGet cmdlget;
        CommandLExist cmdlexist;
        CommandLCount cmdlcount;
        CommandLMove cmdlmove;
        CommandLPos cmdlpos;
        CommandLSearch cmdlsearch;
        CommandLFind cmdlfind;

    public:	
        
        CoreModuleList() : cmdlpush(this),
                           cmdlpop(this),
                           cmdlpopall(this),
                           cmdlget(this),
                           cmdlexist(this),
                           cmdlcount(this),
                           cmdlmove(this),
                           cmdlpos(this),
                           cmdlsearch(this),
                           cmdlfind(this)

        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides list-related commands, LMOVE, LPUSH, LGET and LPOP.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModuleList)
