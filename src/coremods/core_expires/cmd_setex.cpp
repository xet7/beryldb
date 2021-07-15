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
#include "brldb/dbflush.h"
#include "brldb/expires.h"
#include "managers/expires.h"
#include "engine.h"
#include "maker.h"
#include "converter.h"
#include "core_expires.h"

CommandSetex::CommandSetex(Module* Creator) : Command(Creator, "SETEX", 3, 3)
{
          group = 'e';
          syntax = "<seconds> <key> <value>";
}

COMMAND_RESULT CommandSetex::Handle(User* user, const Params& parameters) 
{       
          const std::string& seconds = parameters[0];
          const std::string& key = parameters[1];
          const std::string& value = parameters[2];

          if (!CheckKey(user, key))
          {
              return FAILED;
          }

          /* Similar to a set, setex must have a proper inserting format. */
          
          if (!CheckFormat(user, value))
          {
               return FAILED;
          } 

          if (!CheckValidPos(user, seconds))
          {
               return FAILED;
          }

          /* We convert expiring time to int. */
          
          ExpireHelper::Setex(user, convto_num<unsigned int>(seconds), key, value);
          return SUCCESS;
}

CommandExpireAT::CommandExpireAT(Module* Creator) : Command(Creator, "EXPIREAT", 2, 2)
{
          group = 'e';
          syntax = "<key> <epoch time>";
}

COMMAND_RESULT CommandExpireAT::Handle(User* user, const Params& parameters) 
{    
          const std::string& key = parameters[0];
          const std::string& seconds = parameters[1];

          if (!CheckKey(user, key))
          {
               return FAILED;
          }
          
          if (!CheckValid(user, seconds))
          {
               return FAILED;
          }

          if (!CheckValidPos(user, seconds))
          {
              return FAILED;
          }
                  
          unsigned int exp_usig = convto_num<unsigned int>(seconds);

          if ((time_t)exp_usig < Kernel->Now())
          {
                 user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
                 return FAILED;
          }
          
          ExpireHelper::ExpireAT(user, key, exp_usig);
          return SUCCESS;
}