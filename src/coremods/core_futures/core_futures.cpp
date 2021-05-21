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
#include "core_futures.h"

class CoreModuleFutures : public Module
{
    private:
        
        CommandFuture   cmdfuture;
        CommandTTE	cmdtte;
        CommandExec 	cmdexec;
        CommandFReset   cmdfreset;
        CommandCancel   cmdcancel;
        CommandFResetAll cmdfresetall;
        CommandFKey cmdfkey;
        CommandFValue cmdfvalue;
        CommandFutureCount cmdfcount;
        CommandFutureAT cmdfutureat;
        
    public:
    
        CoreModuleFutures() : cmdfuture(this), cmdtte(this), cmdexec(this), cmdfreset(this),
                              cmdcancel(this), cmdfresetall(this), cmdfkey(this), cmdfvalue(this),
                              cmdfcount(this), cmdfutureat(this)
        {
        
        }

        Version GetDescription() 
        {
                return Version("Provides future and associated commands.", VF_BERYLDB);
        }
};

MODULE_LOAD(CoreModuleFutures)
