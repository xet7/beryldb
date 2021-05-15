/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"

class ModuleChanRestrict : public Module
{
  public:
  
        ModuleChanRestrict()
        {
        
        }
        
        ModuleResult OnUserPreJoin(LocalUser* user, Channel* chan, const std::string& cname)
        {
                 if (!chan)
                 {
                         if (!user->CanPerform('e'))
                         {
                                 user->SendProtocol(ERR_NO_FLAGS, "You need flats in order to create a channel.");
                                 return MOD_RES_STOP;
                         }
                 }
                 
                 return MOD_RES_SKIP;
        }

        Version GetDescription() 
        {
                return Version("Provides ChanRestrict functionalities.", VF_BERYLDB);
        }
};

MODULE_LOAD(ModuleChanRestrict)
