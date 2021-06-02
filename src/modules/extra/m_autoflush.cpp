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

class ModuleAutoFlush : public Module
{
          
  public:

        void OnEveryHour(time_t current)
        {
                if (DBHelper::FlushDB())
                {
                        falert(NOTIFY_DEFAULT, "Database flushed.");
                        return;
                }
                
                falert(NOTIFY_DEFAULT, "Unable to flush database.");
        }
        
        ModuleAutoFlush()
        {
            
        }

        Version GetDescription()
        {
                return Version("Flushes database every one hour.", VF_BERYLDB|VF_OPTCOMMON);
        }
};

MODULE_LOAD(ModuleAutoFlush)
        
