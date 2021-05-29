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
        
//        result = STHelper::Set("conf", "notification::" + user->login);
        
        this->NotifyList.insert(std::make_pair(user, lvl));
        return true;
}

bool Notifier::Has(User* user)
{
        NotifyMap::iterator it = this->NotifyList.find(user);

        if (it == this->NotifyList.end())
        {
                return false;
        }

        return true;
}

void Notifier::Remove(User* user)
{
        this->NotifyList.erase(user);
}

void Notifier::SPush(User* skip, NOTIFY_LEVEL level, const std::string& buff)
{
        if (!this->NotifyList.size() || buff.empty() || !skip || skip->IsQuitting())
        {
             return;
        }

        Event adding(skip, buff, level);
        this->events.push_back(adding);
}

void Notifier::SPush(User* skip, NOTIFY_LEVEL level, const char *fmt, ...) 
{
       std::string buff;
       SCHEME(buff, fmt, fmt);
       this->SPush(skip, level, buff);
}

void Notifier::Push(NOTIFY_LEVEL level, const std::string& buff)
{
        if (!this->NotifyList.size() || buff.empty())
        {
             return;
        }

        Event adding(NULL, buff, level);
        this->events.push_back(adding);
}

void Notifier::Push(NOTIFY_LEVEL level, const char *fmt, ...) 
{
       std::string buff;
       SCHEME(buff, fmt, fmt);
       this->Push(level, buff);
}


void Notifier::Flush()
{
       if (this->events.empty())
       {
            return;
       }

       if (!this->NotifyList.size())
       {
            this->events.clear();
            return;
       }
       
       Event ready = this->events.front();
       
       const NotifyMap& all = this->NotifyList;

       for (NotifyMap::const_iterator uit = all.begin(); uit != all.end(); uit++)
       {
                      User* user = uit->first;
                      
                      if (ready.skip && !ready.skip->IsQuitting())
                      {
                            /* Continues on skipping user. */
                            
                            if (ready.skip == user)
                            {
                                 continue;
                            }
                      }
                      
                      NOTIFY_LEVEL level = uit->second;
                      
                      if (level <= ready.level)
                      {
                             user->SendProtocol(BRLD_NOTIFICATION, Daemon::Format("%s", ready.command.c_str()).c_str());
                      }
       }

       this->events.pop_front();
}




