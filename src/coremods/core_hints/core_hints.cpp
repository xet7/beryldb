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
#include "core_hints.h"

class CoreModHints : public Module
{
    private:
    
        CommandExists 		cmdexists;
        CommandMove 		cmdmove;
        CommandRename 		cmdrename;
        CommandRenameNX         cmdrenamenx;
        CommandDel 		cmddel;
        CommandCopy 		cmdcopy;
        CommandClone 		cmdclone;
        CommandType 		cmdtype;
        CommandDiff 		cmddiff;
        CommandTouch 		cmdtouch;
        CommandNTouch 		cmdntouch;
        CommandTransfer 	cmdtransfer;

    public:
    
        CoreModHints() : cmdexists(this),
                         cmdmove(this),
                         cmdrename(this),   
                         cmdrenamenx(this),
                         cmddel(this),
                         cmdcopy(this),
                         cmdclone(this),
                         cmdtype(this),
                         cmddiff(this),
                         cmdtouch(this),
                         cmdntouch(this),
                         cmdtransfer(this)
        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides global commands.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModHints)

                         
    
