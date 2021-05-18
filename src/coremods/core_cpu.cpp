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
         
         time_t last_lock;
         
         float sample_sincelast;
         float sample_used;
         float usage;
         
  public:

        ModuleCoreCPU()
        {
              last_lock = 0;
              Kernel->Lock = false;
              Kernel->Interval = 2;
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
               if (usage < 10)
               {
                     Kernel->Lock = false;
                     
                     if (Kernel->Interval != 0)
                     {
                          Kernel->Interval = 2;
                     }
                     
                     return;
               }

               if (usage > 10)
               {
                     Kernel->Lock = true;
                     this->last_lock = Kernel->Now();

                     if (Kernel->Interval != 0)
                     {
                          Kernel->Interval = 4;
                     }
               }

               if (usage > 20)
               {
                     Kernel->Lock = true;
                     this->last_lock = Kernel->Now();
                     Kernel->Interval = 10;
               }
               
        }
        
        void OnEveryTwoSeconds(time_t current)
        {
               this->Update();
               CheckUnlock();

               /* We sleep, as there is no activity in the system. */

               if (!Kernel->Clients->GetLocals().size())
               {
                      Kernel->Interval = 1000;
                      Kernel->Lock = true;
                      this->last_lock = Kernel->Now();
               }
        }

        Version GetDescription() 
        {
                return Version("Provides CPU load handling.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleCoreCPU)
        
