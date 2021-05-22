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
#include "brldb/dbmanager.h"
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "brldb/database.h"
#include "managers/databases.h"
#include "converter.h"
#include "engine.h"
#include "core_dbmanager.h"

CommandFlushDB::CommandFlushDB(Module* Creator) : Command(Creator, "FLUSHDB", 0)
{
         requires = 'r';
}

COMMAND_RESULT CommandFlushDB::Handle(User* user, const Params& parameters)
{  
       user->SendProtocol(BRLD_FLUSHING, Daemon::Format("OK: %s", Kernel->Store->Default->GetName().c_str()));
       
       if (DBHelper::FlushDB())
       {
            user->SendProtocol(BRLD_QUERY_OK_DONE, Daemon::Format("FlushDB finished: %s ", Kernel->Store->Default->GetName().c_str()));
            return SUCCESS;
       }
       
       user->SendProtocol(ERR_UNABLE_FLUSH, Daemon::Format("%s: %s.", PROCESS_FALSE.c_str(), Kernel->Store->Default->GetName().c_str()));
       return FAILED;
}