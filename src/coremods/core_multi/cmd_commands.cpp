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
#include "core_multi.h"

CommandMulti::CommandMulti(Module* Creator) : Command(Creator, "MULTI", 0, 0)
{
      group = 'w';
}

COMMAND_RESULT CommandMulti::Handle(User* user, const Params& parameters)
{  
      if (user->MultiRunning)
      {
            user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
            return FAILED;
      }
      
      user->Multi = true;
      user->SendProtocol(BRLD_OK, PROCESS_OK);
      return SUCCESS;
}

CommandMultiReset::CommandMultiReset(Module* Creator) : Command(Creator, "MULTIRESET", 0, 0)
{
      group = 'w';
}

COMMAND_RESULT CommandMultiReset::Handle(User* user, const Params& parameters)
{  
      if (user->Multi)
      {
            user->Multi 	= false;
            user->MultiRunning  = false;
            user->SendProtocol(BRLD_OK, PROCESS_OK);
            
            return SUCCESS;
      }

      user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
      return FAILED;
}

CommandMRUN::CommandMRUN(Module* Creator) : Command(Creator, "MRUN", 0, 0)
{
      group = 'w';
}

COMMAND_RESULT CommandMRUN::Handle(User* user, const Params& parameters)
{  
      return SUCCESS;
}
