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
#include "managers/databases.h"

class CommandAutoFlush : public Command
{
   public:
      
        bool status;
      
        CommandAutoFlush(Module* Creator) : Command(Creator, "AUTOFLUSH", 1, 1)
        {
                syntax = "<on or off>";
        }
        
        COMMAND_RESULT Handle(User* user, const Params& parameters)
        {
                const std::string& arg = parameters[0];
                
                if (arg == "on")
                {
                        status = true;
                }
                else
                {
                        status = false;
                }
                
                user->SendProtocol(BRLD_OK, PROCESS_OK);
                return SUCCESS;
        }
};
        

class ModuleAutoFlush : public Module
{
  private:

        CommandAutoFlush cmd;
          
  public:

        void Initialize()
        {
                cmd.status = false;
        }

        void OnEveryHour(time_t current)
        {
                if (!cmd.status)
                {
                        return;
                }

                const DataMap& dbs = Kernel->Store->DBM->GetDatabases();
                
                for (DataMap::const_iterator i = dbs.begin(); i != dbs.end(); ++i)
                {
                        std::shared_ptr<UserDatabase> db = i->second;
                        
                        if (db->FlushDB())
                        {
                                falert(NOTIFY_DEFAULT, "Database flushed.");
                                return;
                        }
                        else
                        {
                                falert(NOTIFY_DEFAULT, "Unable to flush database.");
                        }
                }
        }
        
        ModuleAutoFlush() : cmd(this)
        {
            
        }

        Version GetDescription()
        {
                return Version("Flushes database every one hour.", VF_BERYLDB|VF_OPTCOMMON);
        }
};

MODULE_LOAD(ModuleAutoFlush)
        
