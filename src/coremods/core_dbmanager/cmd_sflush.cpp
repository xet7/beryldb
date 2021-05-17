/*
 * BerylDB - A modular database.
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
       
       if (!is_number(select))
       {
                 user->SendProtocol(ERR_USE, DBL_NOT_NUM, MUST_BE_NUMERIC.c_str());
                 return FAILED;
       }

       if (!is_positive_number(select))
       {
                user->SendProtocol(ERR_USE, ERR_MUST_BE_POS_INT, MUST_BE_POSIT.c_str());
                return FAILED;
       }

       user->SendProtocol(BRLD_SFLUSH, Daemon::Format("Flushing select ID: %s", select.c_str()).c_str());
       DBHelper::SFlush(user, user->current_db, select);
       
       return SUCCESS;
}