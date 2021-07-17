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
#include "engine.h"

#include "brldb/dbmanager.h"
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "brldb/database.h"
#include "maker.h"
#include "managers/databases.h"
#include "converter.h"
#include "engine.h"
#include "core_dbmanager.h"

CommandSFlush::CommandSFlush(Module* Creator) : Command(Creator, "SFLUSH", 0, 1)
{
         requires = 'r';
         syntax = "<select>";
}

COMMAND_RESULT CommandSFlush::Handle(User* user, const Params& parameters)
{  
       std::string select;
       
       if (parameters.size() > 0)
       {
             select = parameters[0];
       }
       else
       {
            select = user->select;
       }
       
       if (CheckValidPos(user, select))
       {
              return FAILED;
       }
       
       if (!Daemon::CheckRange(user, select, INVALID_RANGE, 1, 100))
       {
              return FAILED;
       }

       DBHelper::SFlush(user, select);
       return SUCCESS;
}