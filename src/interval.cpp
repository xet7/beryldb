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

IntervalManager::IntervalManager() : counter(0)
{

}

bool IntervalManager::RestingStatus()
{
      return this->Sleep;
}

void IntervalManager::SleepMode(bool flag)
{
       this->Sleep = flag;
}

void IntervalManager::Flush()
{
      if (this->Sleep)
      {
            usleep(900);
      }
      
      if (this->counter >= 25)
      {
            /* Activates throttling */
            
            usleep(1);
            return;
      }
      
      this->counter = 0;
}

IntervalManager::~IntervalManager()
{

}