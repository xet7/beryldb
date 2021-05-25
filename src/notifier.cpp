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
#include "notifier.h"

Notifier::Notifier()
{

}

bool Notifier::Add(NOTIFY_LEVEL lvl, User* user)
{
        if (!user || user->IsQuitting())
        {
            return false;
        }
        
//        this->ulevels.insert(std::make_pair(user, lvl));
        return true;
}

bool Notifier::Has(User* user)
{
   return false;
}

void Notifier::Remove(User* user)
{

}


void Notifier::AddTrigger(const std::string& trigger, std::shared_ptr<NotifyStream> stream)
{
        stream->triggers.push_back(trigger);
}


void Notifier::Push(NOTIFY_LEVEL lvl, const std::string& trigger, const std::string& msg)
{

}

void Notifier::Flush()
{
       if (this->events.empty())
       {
           return;
       }
       
       Event ready = this->events.front();
       
       for (StreamMapVec::iterator it = all.begin(); it != all.end(); it++)
       {
              std::string trigger = it->first;
              
              if (trigger != ready.trigger)
              {
                    continue;
              }       
              
              StreamVector streams = it->second;
              
              for (StreamVector::iterator ft = streams.begin(); ft != streams.end(); ft++)
              {
                    std::shared_ptr<NotifyStream> stream = *ft;
                    
                    if (stream->level != ready.level)
                    {
                          continue;
                    }
                    
              }
       }
               
       this->events.pop_front();
       
}




