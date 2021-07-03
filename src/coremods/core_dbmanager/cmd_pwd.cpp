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
#include "brldb/database.h"
#include "brldb/query.h"
#include "managers/tests.h"
#include "converter.h"
#include "engine.h"
#include "core_dbmanager.h"

CommandPWD::CommandPWD(Module* Creator) : Command(Creator, "PWD", 0)
{

}

COMMAND_RESULT CommandPWD::Handle(User* user, const Params& parameters)
{  
       if (user->current_db)
       {
            const std::string path = user->current_db->GetPath().c_str();
            user->SendProtocol(BRLD_CURRENT_DIR, path, Daemon::Format("%s", path.c_str()).c_str());
            return SUCCESS;
       }

       user->SendProtocol(ERR_DB_NOT_SET, PROCESS_NULL);
       return FAILED;
}

CommandDB::CommandDB(Module* Creator) : Command(Creator, "DB", 0)
{

}

COMMAND_RESULT CommandDB::Handle(User* user, const Params& parameters)
{  
       if (user->current_db)
       {
             std::string dbname = user->current_db->GetName();                                  
             user->SendProtocol(BRLD_DB_NAME, dbname);
             return SUCCESS;	
       }
       
       user->SendProtocol(ERR_DB_NOT_SET, PROCESS_NULL);
       return FAILED;
}

CommandChange::CommandChange(Module* Creator) : Command(Creator, "CHANGE", 1, 1)
{
      
}

COMMAND_RESULT CommandChange::Handle(User* user, const Params& parameters)
{  
       const std::string& dbname = parameters[0];
       std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(name);

       if (!database)
       {
             user->SendProtocol(ERR_DB_NOT_FOUND, dbname);
             return FAILED;
       }
       
       user->SendProtocol(BRLD_DB_CHANGED, PROCESS_OK);
       return SUCCESS;
}

CommandDBLIST::CommandDBLIST(Module* Creator) : Command(Creator, "DBLIST", 0, 0)
{
      requires = 'm';
}

COMMAND_RESULT CommandDBLIST::Handle(User* user, const Params& parameters)
{
      const DataMap& dbases = Kernel->Store->DBM->GetDatabases();

      Dispatcher::JustAPI(user, BRLD_DB_BEGIN);
       
      for (DataMap::const_iterator it = dbases.begin(); it != dbases.end(); ++it)
      {
            std::shared_ptr<UserDatabase> udb = it->second;
            
            const std::string& dbname = udb->GetName();
            const std::string& dbpath = udb->GetPath();
            Dispatcher::CondList(user, BRLD_DB_ITEM, dbname, dbpath);
      }     

      Dispatcher::JustAPI(user, BRLD_DB_END);
      
      return SUCCESS;
}

CommandDBCreate::CommandDBCreate(Module* Creator) : Command(Creator, "DBCREATE", 1, 1)
{
      requires = 'm';
      syntax = "<name>";
}

COMMAND_RESULT CommandDBCreate::Handle(User* user, const Params& parameters)
{
      const std::string& dbname = parameters[0];
      
      if (Kernel->Store->DBM->Create(dbname, dbname))
      {			
          Kernel->Store->DBM->Load(dbname, dbname);

          user->SendProtocol(BRLD_DB_CREATED, PROCESS_OK);
          return SUCCESS;
      }
      
      user->SendProtocol(ERR_DB_EXISTS, PROCESS_FALSE);
      return FAILED;
}

CommandDBDelete::CommandDBDelete(Module* Creator) : Command(Creator, "DBDELETE", 1, 1)
{
      requires = 'm';
      syntax = "<name>";
}

COMMAND_RESULT CommandDBDelete::Handle(User* user, const Params& parameters)
{
      const std::string& dbname = parameters[0];
      
       std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);

       if (!database)
       {
             user->SendProtocol(ERR_DB_NOT_FOUND, dbname);
             return FAILED;
       }
      
      const std::string& dbpath = database->GetPath();
      
      if (Kernel->Store->DBM->Delete(dbname))
      {                 
            user->SendProtocol(BRLD_DB_DELETED, PROCESS_OK);
            FileSystem::remove_directory(dbpath.c_str());
            return SUCCESS;
      }
      
      user->SendProtocol(ERR_DB_NOT_EXISTS, PROCESS_FALSE);
      return FAILED;
}

CommandDBTest::CommandDBTest(Module* Creator) : Command(Creator, "TEST", 0, 0)
{

}

COMMAND_RESULT CommandDBTest::Handle(User* user, const Params& parameters)
{
      user->SendProtocol(BRLD_TEST, PROCESS_OK);
      TestHelper::Dump(user);
      return SUCCESS;
}
