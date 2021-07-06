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

CommandFlushAll::CommandFlushAll(Module* Creator) : Command(Creator, "FLUSHALL", 0, 1)
{
       requires = 'r';
}

COMMAND_RESULT CommandFlushAll::Handle(User* user, const Params& parameters)
{  
     DataMap dbs = Kernel->Store->DBM->GetDatabases();

     for (DataMap::iterator i = dbs.begin(); i != dbs.end(); ++i)
     {
               std::shared_ptr<UserDatabase> db = i->second;
               db->FlushDB();
     }
                
     user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);                
     return SUCCESS;
}

CommandFlushDB::CommandFlushDB(Module* Creator) : Command(Creator, "FLUSHDB", 0, 1)
{
       requires = 'r';
}

COMMAND_RESULT CommandFlushDB::Handle(User* user, const Params& parameters)
{  
       const std::string& dbname = parameters[0];
       
       std::shared_ptr<UserDatabase> database;
       
       if (parameters.size())
       {
             database = Kernel->Store->DBM->Find(dbname);
       }
       else
       {
            database = user->current_db;
       }
       
       if (!database)
       {
            user->SendProtocol(ERR_DB_NOT_SET, PROCESS_NULL);
            return FAILED;
       }
       
       if (DBHelper::FlushDB(database, true))
       {
             user->SendProtocol(BRLD_FLUSHED, PROCESS_OK);
             return SUCCESS;
       }

       sfalert(user, NOTIFY_DEFAULT, "Flushed database: %s", user->current_db->GetName().c_str());      
       user->SendProtocol(ERR_UNABLE_FLUSH, PROCESS_FALSE);
       return FAILED;
}

