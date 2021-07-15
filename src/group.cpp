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
#include "managers/settings.h"

GroupManager::GroupManager()
{

}

GroupManager::~GroupManager()
{

}

std::shared_ptr<Group> GroupManager::Find(const std::string& name)
{
        GroupMap::iterator iter = this->GroupList.find(name);

        if (iter == this->GroupList.end())
        {
                return NULL;
        }

        return iter->second;
}

bool GroupManager::Add(const std::string& gname, const std::string& flags)
{
      std::string name = gname;
      
      std::transform(name.begin(), name.end(), name.begin(), ::tolower);

      /* Check if name is taken. */
      
      if (this->Find(name))
      {
           return false;
      }
      
      std::shared_ptr<Group> New = std::make_shared<Group>();
      New->created = Kernel->Now();
      New->name = name;
      New->UpdateFlags(flags);
      
      this->GroupList.insert(std::make_pair(name, New));
      
      bprint(DONE, "Adding group: %s", name.c_str());
      
      return true;
}

bool Group::UpdateFlags(const std::string& flags)
{
      this->Init();
      this->rawflags += flags;
      
      for (auto it = this->rawflags.cbegin() ; it != this->rawflags.cend(); ++it)
      {
            if (*it == 'k')
            {
                  this->can_keys = true;
                  continue;
            }    
            
            if (*it == 'z')
            {
                  this->can_do_all = true;
                  continue;                  
            }
            
            if (*it == 'l')
            {
                  this->can_list = true;
                  continue;                  
            }
            
            if (*it == 'g')
            {
                  this->can_geo = true;
                  continue;                  
            }
            
            if (*it == 'v')
            {
                  this->can_vector = true;
                  continue;                 
            }
            
            if (*it == 'e')
            {
                  this->can_expire = true;
                  continue;                  
            }
            
            if (*it == 'f')
            {
                  this->can_future = true;
                  continue;                 
            }
            
            if (*it == 'm')
            {
                  this->can_map = true;
                  continue;                 
            }
            
            if (*it == 'x')
            {
                  this->can_mmap = true;
                  continue;                 
            }
            
            if (*it == 'p')
            {
                  this->can_publish_chan = true;
                  continue;                
            }
            
            if (*it == 'c')
            {
                  this->can_publish_clients = true;
                  continue;                  
            }

            if (*it == 'h')
            {
                  this->can_hints = true;
                  continue;
            }
            
            if (*it == 'w')
            {
                  this->can_dual_exec = true;
                  continue;
            }
            
            bprint(ERROR, "Invalid flag");
            return false;
      } 
      
      STHelper::Set("groups", this->name, flags);
      return true;
}

bool GroupManager::ResetFlags(const std::string& name)
{
      std::shared_ptr<Group> group = this->Find(name);
      
      if (!group)
      {
           return false;
      }
      
      /* Reset flags. */
      
      group->Init();
      group->rawflags = "";
      STHelper::Set("groups", name, "");
      return true;
}

bool GroupManager::Delete(const std::string& gname)
{
      std::string name = gname;
      std::transform(name.begin(), name.end(), name.begin(), ::tolower);

      std::shared_ptr<Group> InUse = this->Find(name);
      
      if (!InUse)
      {
            return false;
      }

      STHelper::Delete("groups", name);
      this->GroupList.erase(name);
      
      Args users = STHelper::HKeys("userlist");
      
      for (Args::iterator u = users.begin(); u != users.end(); u++)
      {
            std::string login = *u;
            std::string usergrups = login + "/groups";
            
            Args groups = STHelper::HKeys(usergrups);

            for (Args::iterator i = groups.begin(); i != groups.end(); i++)
            {
                std::string gdelete = *i;
                
                if (gdelete == name)
                {
                     STHelper::Delete(usergrups, name);
                }
            }
      }
      
      Kernel->Clients->RemoveGroup(InUse);
      return true;
}

void Group::Init()
{
      can_do_all 		= 	false;
      can_keys 			=	false;
      can_list 			= 	false;
      can_geo			=	false;
      can_vector		=	false;
      can_expire		=	false;
      can_future		=	false;
      can_map			=	false;
      can_mmap			=	false;
      can_publish_chan		=	false;
      can_publish_clients 	=	false;
      can_hints			=	false;
      can_dual_exec		=	false;
}

Group::Group() 
{
      
}

Group::~Group()
{

}


bool Group::CanDo(unsigned char flag)
{
      if (can_do_all)
      {
          return true;
      }
      
      if (flag == 'k')
      {
            if (can_keys)
            {
                 return true;
            }
      }
      
      if (flag == 'l')
      {
            if (can_list)
            {
                 return true;
            }
      }
      
      if (flag == 'g')
      {
            if (can_geo)
            {
                 return true;
            }
      }
      
      if (flag == 'v')
      {
            if (can_vector)
            {
                 return true;
            }
      }
      
      if (flag == 'e')
      {
            if (can_expire)
            {
                 return true;
            }
      }
      
      if (flag == 'f')
      {
            if (can_future)
            {
                 return true;
            }
      }
      
      if (flag == 'm')
      {
            if (can_map)
            {
                 return true;
            }
      }
      
      if (flag == 'x')
      {
            if (can_mmap)
            {
                 return true;
            }
      }
      
      if (flag == 'p')
      {
            if (can_publish_chan)
            {
                 return true;
            }
      }
      
      if (flag == 'c')
      {
            if (can_publish_clients)
            {
                 return true;
            }
      }

      if (flag == 'h')
      {
            if (can_hints)
            {
                 return true;
            }
      }

      if (flag == 'w')
      {
            if (can_dual_exec)
            {
                 return true;
            }
      }

      return false;
}

void GroupManager::Reset()
{
      for (GroupMap::iterator i = GroupList.begin(); i != GroupList.end(); i++)
      {
                  std::shared_ptr<Group> InUse = i->second;
                  InUse = nullptr;
      }

      this->GroupList.clear();
}
