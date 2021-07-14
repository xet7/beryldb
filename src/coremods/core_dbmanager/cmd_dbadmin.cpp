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
#include "managers/databases.h"
#include "managers/globals.h"
#include "converter.h"
#include "engine.h"
#include "managers/settings.h"
#include "core_dbmanager.h"

CommandPWD::CommandPWD(Module* Creator) : Command(Creator, "PWD", 0)
{

}

COMMAND_RESULT CommandPWD::Handle(User* user, const Params& parameters)
{  
       if (!BASE_PATH.empty())
       {
            const std::string path = BASE_PATH.c_str();
            user->SendProtocol(BRLD_INPUT, path);
            return SUCCESS;
       }

       user->SendProtocol(ERR_INPUT2, ERR_DB_NOT_SET, PROCESS_NULL);
       return FAILED;
}

CommandDefault::CommandDefault(Module* Creator) : Command(Creator, "DEFAULT", 0)
{

}

COMMAND_RESULT CommandDefault::Handle(User* user, const Params& parameters)
{  
       std::string dbname = STHelper::Get("dbconf", "dbdefault");
       
       if (dbname.empty())
       {
             user->SendProtocol(ERR_INPUT, PROCESS_NULL);
             return FAILED;
       }
       
       user->SendProtocol(BRLD_INPUT, dbname);
       return SUCCESS;
}

CommandDB::CommandDB(Module* Creator) : Command(Creator, "DB", 0)
{

}

COMMAND_RESULT CommandDB::Handle(User* user, const Params& parameters)
{  
       if (user->GetDatabase())
       {
             std::string dbname = user->GetDatabase()->GetName();                                  
             user->SendProtocol(BRLD_QUERY_OK, dbname);
             return SUCCESS;	
       }
       
       user->SendProtocol(ERR_INPUT, PROCESS_NULL);
       return FAILED;
}

CommandChange::CommandChange(Module* Creator) : Command(Creator, "CHANGE", 1, 1)
{
      
}

COMMAND_RESULT CommandChange::Handle(User* user, const Params& parameters)
{  
       std::string dbname = parameters[0];
       std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);

       if (!database)
       {
             user->SendProtocol(ERR_INPUT, DB_NULL);
             return FAILED;
       }

       user->SetDatabase(database);
       STHelper::Set("dbuser", user->login, dbname);
       user->SendProtocol(BRLD_INPUT, PROCESS_OK);
       return SUCCESS;
}

CommandDBLIST::CommandDBLIST(Module* Creator) : Command(Creator, "DBLIST", 0, 0)
{
      requires = 'r';
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
      requires = 'r';
      syntax = "<name>";
}

COMMAND_RESULT CommandDBCreate::Handle(User* user, const Params& parameters)
{
      std::string dbname = parameters[0];

      /* 'dbdefault' is a reserved database name. */
      
      if (dbname == "dbdefault" || dbname == "core")
      {
             user->SendProtocol(ERR_INPUT2, ERR_DB_EXISTS, PROCESS_FALSE);
             return FAILED;
      }
      
      std::transform(dbname.begin(), dbname.end(), dbname.begin(), ::tolower);
      
      if (Kernel->Store->DBM->Create(dbname, dbname))
      {			
             Kernel->Store->DBM->Load(dbname);
             user->SendProtocol(BRLD_DB_CREATED, PROCESS_OK);
             sfalert(user, NOTIFY_DEFAULT, "Added database: %s", dbname.c_str());      
             
             if (user->GetDatabase() == nullptr)
             {
                  std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);
                  
                  if (database)
                  {
                        user->SetDatabase(database);
                        STHelper::Set("dbuser", user->login, dbname);
                  }
             }
             
             return SUCCESS;
      }
      
      user->SendProtocol(ERR_INPUT2, ERR_DB_EXISTS, PROCESS_FALSE);
      return FAILED;
}

CommandDBDelete::CommandDBDelete(Module* Creator) : Command(Creator, "DBDELETE", 1, 1)
{
      requires = 'r';
      syntax = "<name>";
}

COMMAND_RESULT CommandDBDelete::Handle(User* user, const Params& parameters)
{
      std::string dbname = parameters[0];
      
      std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);

      if (!database)
      {
             user->SendProtocol(ERR_INPUT, PROCESS_NULL);
             return FAILED;
      }
      
      /* We cannot remove a database whenever is locked. */
      
      if (database->IsClosing())
      {
             user->SendProtocol(ERR_INPUT, DATABASE_BUSY);
             return FAILED;
      }
      
      GlobalHelper::DatabaseReset(user, database->GetName());
      return SUCCESS;
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

CommandDBSetDefault::CommandDBSetDefault(Module* Creator) : Command(Creator, "SETDEFAULT", 1, 1)
{
      requires = 'r';
      syntax = "<db name>";
}

COMMAND_RESULT CommandDBSetDefault::Handle(User* user, const Params& parameters)
{
      const std::string& dbname = parameters[0];

      std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(dbname);

      if (!database)
      {
             user->SendProtocol(ERR_INPUT2, ERR_DB_NOT_FOUND, PROCESS_NULL);
             return FAILED;
      }

      sfalert(user, NOTIFY_DEFAULT, "Setting default database to: %s", database->GetName().c_str());

      DBManager::SetDefault(dbname);
      user->SendProtocol(BRLD_INPUT, PROCESS_OK);
      return SUCCESS;
}