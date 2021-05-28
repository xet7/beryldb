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
#include "core_operations.h"

class CoreModOperations : public Module
{
    private:
    
        CommandIncr cmdincr;
        CommandIncrBy cmdincrby;
        CommandDecr cmddecr;
        CommandDecrBy cmddecrby;
        CommandAvg cmdavg;
        CommandDiv cmddiv;
        CommandMult cmdmult;
        
    public:     
        
        CoreModOperations() : cmdincr(this), cmdincrby(this), cmddecr(this), 
                              cmddecrby(this), cmdavg(this), cmddiv(this),
                              cmdmult(this)
        {
        
        }

        Version GetDescription() 
        {
                return Version("Provides operational-based commands: INCR, DECR, INCRBY, DECRBY, AVG, MULT and DIV.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(CoreModOperations)
        
