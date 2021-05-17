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
#include "engine.h"
#include "converter.h"
#include "core_futures.h"

CommandFuture::CommandFuture(Module* Creator) : Command(Creator, "FUTURE", 3, 3)
{
          syntax = "<seconds> <key> <value>";
}

COMMAND_RESULT CommandFuture::Handle(User* user, const Params& parameters) 
{       
          const std::string& seconds = parameters[0];
          const std::string& key = parameters[1];
          const std::string& value = parameters[2];
          
          if (!is_number(seconds))
          {
                 user->SendProtocol(ERR_FUTURE, seconds, MUST_BE_NUMERIC.c_str());
                 return FAILED;
          }

          if (!is_positive_number(seconds))
          {
                 user->SendProtocol(ERR_FUTURE, seconds, MUST_BE_POSIT.c_str());
                 return FAILED;
          }
  
          if (!Daemon::CheckFormat(user, value))
          {
               return FAILED;
          }   

          unsigned int exp_usig = convto_num<unsigned int>(seconds);
          Kernel->Store->Futures->Add(exp_usig, key, value, user->select, false);
          user->SendProtocol(BRLD_FUTURE_ADD, key, value, "New future added.");
          return SUCCESS;
}
