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

class ModuleCoreCPU : public Module
{
  private:
         
         float sample_sincelast;
         float sample_used;
         float usage;
         
  public:

        ModuleCoreCPU() : sample_used(0), usage(0)
        {

        }
        
        void OnUnloadModule(Module* module)
        {	

        }
        
        void Initialize()
        {
                      Kernel->Interval->SleepMode(false);   
        }
        
        void Update()
        {
              rusage R;

              if (!getrusage(RUSAGE_SELF,&R)) 
              {
                      sample_sincelast = (Kernel->Now() - Kernel->Stats.LastSampled.tv_sec) * 1000000 + (Kernel->TimeStamp() - Kernel->Stats.LastSampled.tv_nsec) / 1000;
                      sample_used = ((R.ru_utime.tv_sec - Kernel->Stats.LastCPU.tv_sec) * 1000000 + R.ru_utime.tv_usec - Kernel->Stats.LastCPU.tv_usec);
                      usage = (sample_used / sample_sincelast) * 100;
              }
        }
        
        /* 
         * Checks whether we should unlock a previous
         * lock. This function should ran only when
         * locked.
         */    
                 
        void CheckUnlock()
        {

        }
        
        void OnPostConnect(User* user)
        {
              if (Kernel->Interval->RestingStatus())
              {
                      Kernel->Interval->SleepMode(false);   
              }
        }
        
        void OnEveryTwoSeconds(time_t current)
        {
              if (!Kernel->Clients->GetLocals().size())
              {
                    Kernel->Interval->SleepMode(true);
                    return;
              }
              
              this->Update();
              
              if (this->usage > 10)
              {
                   usleep(10000);
              }
              
              if (this->usage > 8)
              {
                   Kernel->Interval->SleepMode(true);
                   return;
              }
              
              if (this->usage > 6)
              {
                    Kernel->Interval->SleepMode(true);
                    return;
              }
              
              Kernel->Interval->SleepMode(false);
              
        }

        Version GetDescription() 
        {
                return Version("Provides CPU load handling.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleCoreCPU)
        
