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
#include "brldb/dbflush.h"
#include "brldb/expires.h"
#include "managers/keys.h"
#include "engine.h"
#include "converter.h"
#include "core_expires.h"

CommandExpire::CommandExpire(Module* Creator) : Command(Creator, "EXPIRE", 2, 2)
{
          syntax = "<key> <seconds>";
}

COMMAND_RESULT CommandExpire::Handle(User* user, const Params& parameters) 
{       
          const std::string& key = parameters[0];
          const std::string& exp_str = parameters[1];
          
          if (!is_number(exp_str))
          {
                 user->SendProtocol(ERR_EXPIRE, exp_str, MUST_BE_NUMERIC.c_str());
                 return FAILED;
          }
          
          if (!is_positive_number(exp_str))
          {
                 user->SendProtocol(ERR_EXPIRE, exp_str, MUST_BE_POSIT.c_str());
                 return FAILED;
          }
                  
          unsigned int exp_usig = convto_num<unsigned int>(exp_str);
          KeyHelper::Expire(user, user->current_db, user->select, key, TYPE_EXPIRE, exp_usig);
          return SUCCESS;
}