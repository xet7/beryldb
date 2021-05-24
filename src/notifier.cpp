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

bool NotifyStream::Add(User* user)
{
        if (!user || user->IsQuitting())
        {
            return false;
        }
        
        this->users.push_back(user);
        return true;
}

bool NotifyStream::Has(User* user)
{
        if (std::find(users.begin(), users.end(), user) != users.end())
        {
             return true;
        }
        
        return false;
}

void NotifyStream::Remove(User* user)
{

}

void Notifier::Create(const std::string& name)
{
  
}

std::shared_ptr<NotifyStream> Notifier::Find(const std::string& name)
{
        StreamMap::iterator it = this->AllStreams.find(name);

        if (it == this->AllStreams.end())
        {
                return nullptr;
        }

        return it->second;
}

void Notifier::AttachType(const std::string &type, std::shared_ptr<NotifyStream> stream)
{
        ActiveStreams[type].push_back(stream);
}


