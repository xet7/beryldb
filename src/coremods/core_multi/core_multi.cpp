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

class CoreModuleMulti : public Module
{
    private:
        
        CommandMulti 		cmdmulti;
        CommandMRUN 		cmdmrun;
        CommandMultiReset 	cmdmultireset;
        
    public:     
        
        CoreModuleMulti() : cmdmulti(this), 
                            cmdmrun(this), 
                            cmdmultireset(this)
        {

        }

        Version GetDescription() 
        {
                return Version("Provides multi support.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModuleMulti)
