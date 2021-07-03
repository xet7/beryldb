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

#pragma once

class ExportAPI IntervalManager : public safecast<IntervalManager>
{
     friend class CommandQueue;
     
     private:
             
        unsigned int counter;
        
        bool Sleep;
        
     public:
        
        /* Constructor */
        
        IntervalManager();
        
        /* 
         * Runs in everyloop awaiting for sleeping requests.
         * Flush() is actively looking for proper usage of resources.
         */    
                 
        void Flush();
        
        /* 
         * Sets Beryl to rest. This function calls usleep() in order to
         * utilize less resources, when needed.
         * 
         * @parameters:
	 *
	 *         · flag: True if sleeping is requested, false if not.
         */            
         
        void SleepMode(bool flag);
        
        /* 
         * Resting status: Returns current status of this->Sleep.
         * 
         * @return:
 	 *
         *         · true: Resting is true.
         *         · false: Not resting.
         */    
         
        bool RestingStatus();
        
        /* Resets counters */
        
        void Reset()
        {
             this->counter = 0;
        }
};