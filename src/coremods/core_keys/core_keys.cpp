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
#include "core_keys.h"

class CoreModKeys : public Module
{
    private:
    
        CommandSet cmdset;
        CommandGet cmdget;
        CommandDel cmddel;
        CommandStrlen cmdstrlen;
        CommandFind cmdfind;
        CommandCount cmdcount;
        CommandMove cmdmove;
        CommandExists cmdexists;
        CommandGetDel cmdgetdel;
        CommandGetSet cmdgetset;
        CommandGetRange cmdgetrange;
        CommandRename cmdrename;
        CommandRenameNX cmdrenamenx;
        CommandCopy cmdcopy;
        CommandRKey cmdrkey;
        CommandTouch cmdtouch;
        CommandNTouch cmdntouch;
        CommandTCount cmdtcount;
        CommandConcat cmdconcat;
        CommandSConcat cmdsconcat;
        CommandAppend cmdappend;
        CommandSetNX cmdsetnx;
        CommandSetTX cmdsettx;
        CommandSearch cmdsearch;
        
    public:	
        
        CoreModKeys() : cmdset(this), 
                        cmdget(this),	
                        cmddel(this),
                        cmdstrlen(this),
                        cmdfind(this),
                        cmdcount(this),
                        cmdmove(this),
                        cmdexists(this),
                        cmdgetdel(this),
                        cmdgetset(this),
                        cmdgetrange(this),
                        cmdrename(this),
                        cmdrenamenx(this),
                        cmdcopy(this),
                        cmdrkey(this),
                        cmdtouch(this),
                        cmdntouch(this),
                        cmdtcount(this),
                        cmdconcat(this),
                        cmdsconcat(this),
                        cmdappend(this),
                        cmdsetnx(this),
                        cmdsettx(this),
                        cmdsearch(this)

        {
        
        }
        
        Version GetDescription() 
        {
                return Version("Provides commands related to keys, such as SET, DEL and GET.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModKeys)
