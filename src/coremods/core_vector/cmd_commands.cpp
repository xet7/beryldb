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
#include "core_vector.h"

CommandVBack::CommandVBack(Module* Creator) : Command(Creator, "VBACK", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandVBack::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       VectorHelper::Back(user, key);
       return SUCCESS;  
}

CommandVFront::CommandVFront(Module* Creator) : Command(Creator, "VFRONT", 1, 1)
{
         group = 'l';
         syntax = "<key>";
}

COMMAND_RESULT CommandVFront::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       if (!CheckKey(user, key))
       {
            return FAILED;
       }

       VectorHelper::Front(user, key);
       return SUCCESS;  
}
