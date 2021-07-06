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
#include "managers/keys.h"
#include "managers/globals.h"
#include "engine.h"
#include "core_keys.h"


CommandWDel::CommandWDel(Module* Creator) : Command(Creator, "WDEL", 1, 1)
{
         syntax = "<%key>";
}

COMMAND_RESULT CommandWDel::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!Daemon::CheckFormat(user, key))
       {
            return FAILED;
       }
       
       KeyHelper::WDelete(user, key);
       return SUCCESS;
}
