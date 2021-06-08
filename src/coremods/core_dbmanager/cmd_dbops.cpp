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
       if (DBHelper::FlushDB())
       {
            user->SendProtocol(BRLD_FLUSHED, PROCESS_OK);
            return SUCCESS;
       }

       sfalert(user, NOTIFY_DEFAULT, "Flushed database: %s", Kernel->Store->Default->GetName().c_str());      
       user->SendProtocol(ERR_UNABLE_FLUSH, PROCESS_FALSE);
       return FAILED;
}

CommandSwapDB::CommandSwapDB(Module* Creator) : Command(Creator, "SWAPDB", 2, 2)
{
         requires = 'r';
         syntax = "<select1> <select2>";
}

COMMAND_RESULT CommandSwapDB::Handle(User* user, const Params& parameters)
{  

       const std::string& db1 = parameters[0];
       const std::string& db2 = parameters[1];
       
       if (!is_number(db1))
       {
                 user->SendProtocol(ERR_USE, DBL_NOT_NUM, MUST_BE_NUMERIC.c_str());
                 return FAILED;
       }

       if (!is_positive_number(db1))
       {
                user->SendProtocol(ERR_USE, ERR_MUST_BE_POS_INT, MUST_BE_POSIT.c_str());
                return FAILED;
       }

       if (!Daemon::CheckRange(user, db1, "Must be a value between 1 and 100", 1, 100))
       {
               return FAILED;
       }
       
       if (!is_number(db2))
       {
                 user->SendProtocol(ERR_USE, DBL_NOT_NUM, MUST_BE_NUMERIC.c_str());
                 return FAILED;
       }

       if (!is_positive_number(db2))
       {
                user->SendProtocol(ERR_USE, ERR_MUST_BE_POS_INT, MUST_BE_POSIT.c_str());
                return FAILED;
       }

       if (!Daemon::CheckRange(user, db2, "Must be a value between 1 and 100", 1, 100))
       {
               return FAILED;
       }

       sfalert(user, NOTIFY_DEFAULT, "Database swap: %s <=> %s", db1.c_str(), db2.c_str());
       
       DBHelper::SwapDB(user, user->current_db, db1, db2);
       return SUCCESS;
}

