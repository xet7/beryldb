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
#include "login.h"
#include "modules/encrypt.h"

std::shared_ptr<Session> SetFlags(std::shared_ptr<Session> New, const std::string& flags)
{
       /* Session has no flags by default. */
      
       New->can_admin = false;
       New->can_execute = false;
       New->can_manage = false;
       
       for (auto it = flags.cbegin() ; it != flags.cend(); ++it) 
       {
            if (*it == 'r')
            {
                  New->can_admin = true;
            }

            if (*it == 'e')
            {
                  New->can_execute = true;
            }

            if (*it == 'm')
            {
                  New->can_manage = true;
            }
      }
     
      return New;
}

SessionManager::SessionManager()
{

}

Session::Session() : can_admin(false)
                   , can_execute(false)
                   , can_manage(false)
{

}

void SessionManager::DetermineLifetime(const std::string& login)
{
     unsigned int counter = Kernel->Clients->CountLogin(login);
     
     /* Nothing to delete. */
     
     if (counter > 0)
     {
            return;
     }
         
     for (SessionsMap::iterator i = sessions.begin(); i != sessions.end(); )
     {
            std::shared_ptr<Session> session = i->second;
                
            if (i->first != login)
            {
                     i++;
                     continue;
            }
                
            i->second.reset();
            this->sessions.erase(i++);
            break;
     }
}

std::shared_ptr<Session> SessionManager::Add(const std::string& login, const std::string& flags)
{
    if (login.empty())
    {
         return nullptr;
    } 
    
    std::shared_ptr<Session> New = std::make_shared<Session>();
    
    SetFlags(New, flags);
    New->rawflags = flags;
    
    this->sessions.insert(std::make_pair(login, New));
    return New;
}

void SessionManager::AttachExternal(const std::string& login, const std::string& flags)
{
     UserVector logins = Kernel->Clients->FindLogin(login);

     for (UserVector::iterator o = logins.begin(); o != logins.end(); ++o)
     {
           User* user = *o;
           this->Attach(user, login, flags);
     }
}

void SessionManager::Attach(User* user, const std::string& login, const std::string& flags)
{
     std::shared_ptr<Session> attach = this->Find(login);
      
      if (attach == nullptr)
      {
            attach = this->Add(login, flags);
      }
      else
      {
            attach = SetFlags(attach, flags);
      }
            
      if (user != NULL)
      {
             user->session = attach;
             NOTIFY_MODS(OnAdmin, (user, flags));
      }
}

std::shared_ptr<Session> SessionManager::Find(const std::string& login)
{
     std::map<std::string, std::shared_ptr<Session>>::iterator iter = this->sessions.find(login);

     if (iter == this->sessions.end())
     {
             return nullptr;
     }

     return iter->second;
}

void SessionManager::NotifyFlags(const std::string& login, const std::string& flags)
{
      UserVector logins = Kernel->Clients->FindLogin(login);

      for (UserVector::iterator o = logins.begin(); o != logins.end(); ++o)
      {
           User* user = *o;
           user->SendProtocol(BRLD_YOUR_FLAGS, user, flags.c_str());
      }
}

LoginCache::LoginCache()
{

}

void LoginCache::Reset()
{
     this->logins.clear();
}

bool LoginCache::RemoveLastCache()
{
    /* Remove last element. */
    
    if (this->logins.empty())
    {
          return false;
    }
    
    LoginMap::iterator it = logins.begin();
    std::advance(it, static_cast<int>(logins.size())-1); 
    logins.erase(it);
    return true;
}

void LoginCache::Add(const std::string& user, const std::string& pass)
{
      this->logins.insert(std::make_pair(user, pass));
  
      /* This cache should not have more than 1024 entries. */
        
      if (logins.size() >= 1024)
      {
          this->RemoveLastCache();
      }
}

void LoginCache::Remove(const std::string& user)
{
     logins.erase(user);
}

signed int LoginCache::InCache(const std::string& user, const std::string& pass)
{
    for (LoginMap::iterator i = logins.begin(); i != logins.end(); ++i)
    {
         if (i->first == user)
         {
                 HashProvider* provider = Kernel->Modules->DataModule<HashProvider>("hash/bcrypt");

                 if (!provider)
                 {
                       return -1;
                 }

                 if (provider->Compare(pass, i->second))
                 {
                       return 1;
                 }
                 else
                 {
                       /* Login found, but wrong password. */
                      
                       return -1;
                 }
         }
    }
    
    /* Login not found. */
    
    return 0;
}

bool LoginCache::InCache(const std::string& user)
{
        LoginMap::iterator it = this->logins.find(user);

        if (it == this->logins.end())
        {
                return false;
        }

        return true;
}