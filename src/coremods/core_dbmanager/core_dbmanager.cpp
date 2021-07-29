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
#include "core_dbmanager.h"

class CoreModDBManager : public Module
{
    private:
    
        CommandUse cmduse;
        CommandCurrent cmdcurrent;
        CommandFlushDB cmdflushdb;
        CommandPWD cmdpwd;
        CommandDBSize cmddbsize;
        CommandDBReset cmddbreset;
        CommandSFlush cmdsflush;
        CommandUsing cmdusing;
        CommandDB cmddb;
        CommandChange cmdchange;
        CommandDBLIST dblist;
        CommandDBCreate cmddbcreate;
        CommandDBDelete cmddbdelete;
        CommandDefault cmdefault;
        CommandDBTest cmddbtest;
        CommandDBSetDefault cmdsdfault;
        CommandFlushAll cmdflushall;

    public:     
        
        CoreModDBManager() : cmduse(this), 
                             cmdcurrent(this), 
                             cmdflushdb(this), 
                             cmdpwd(this),
                             cmddbsize(this), 
                             cmddbreset(this), 
                             cmdsflush(this), 
                             cmdusing(this),
                             cmddb(this), 
                             cmdchange(this),
                             dblist(this), 
                             cmddbcreate(this), 
                             cmddbdelete(this), 
                             cmdefault(this), 
                             cmddbtest(this), 
                             cmdsdfault(this),
                             cmdflushall(this)
        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides database admin commands.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModDBManager)
