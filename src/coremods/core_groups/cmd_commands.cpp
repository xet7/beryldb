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
#include "core_groups.h"
#include "managers/maps.h"
#include "managers/user.h"
#include "managers/settings.h"

CommandGroupAdd::CommandGroupAdd(Module* Creator) : Command(Creator, "GROUPADD", 1, 1)
{
        requires = 'r';
        syntax = "<name>";
}

COMMAND_RESULT CommandGroupAdd::Handle(User* user, const Params& parameters)
{
       const std::string& gname = parameters[0];
       
       if (!Kernel->Engine->IsGroup(gname))
       {
               user->SendProtocol(ERR_INPUT2, ERR_BAD_FORMAT, WRONG_AGENT);
               return FAILED;
       }
       
       /* A new group has 'z' flags by default. */
       
       if (Kernel->Groups->Add(gname, DEFAULT_GFLAGS))
       {
             user->SendProtocol(BRLD_INPUT, PROCESS_OK);
             return SUCCESS;
       }
       
       user->SendProtocol(ERR_INPUT, PROCESS_ALREADY);
       return FAILED;
}

CommandGroups::CommandGroups(Module* Creator) : Command(Creator, "GROUPS", 0, 0)
{
        requires = 'r';
}

COMMAND_RESULT CommandGroups::Handle(User* user, const Params& parameters) 
{
         GroupMap& l_groups = Kernel->Groups->GetList();

         Dispatcher::JustAPI(user, BRLD_GROUP_BEGIN);

         for (GroupMap::iterator it = l_groups.begin(); it != l_groups.end(); ++it)
         {
               std::string gname = it->first;
               user->SendProtocol(BRLD_GROUP_ITEM, gname.c_str());
         }

         Dispatcher::JustAPI(user, BRLD_GROUP_END);
         return SUCCESS;
}

CommandDelGroup::CommandDelGroup(Module* Creator) : Command(Creator, "DELGROUP", 1, 1)
{
        requires = 'r';
        syntax = "<name>";
}

COMMAND_RESULT CommandDelGroup::Handle(User* user, const Params& parameters) 
{
       const std::string& gname = parameters[0];
       
       if (gname == "default")
       {
             user->SendProtocol(ERR_INPUT, ENTRY_PROTECTED);
             return FAILED;
       }

       if (Kernel->Groups->Delete(gname))
       {
             user->SendProtocol(BRLD_INPUT, PROCESS_OK);
             return SUCCESS;
       }

       user->SendProtocol(ERR_INPUT, PROCESS_NULL);
       return FAILED;
}

CommandGShow::CommandGShow(Module* Creator) : Command(Creator, "GSHOW", 1, 1)
{
        requires = 'r';
        syntax = "<name>";
}

COMMAND_RESULT CommandGShow::Handle(User* user, const Params& parameters) 
{
       const std::string& gname = parameters[0];
       
       std::shared_ptr<Group> a_group = Kernel->Groups->Find(gname);
       
       if (!a_group)
       {
             user->SendProtocol(ERR_INPUT, PROCESS_NULL);
             return FAILED;
       }
       
       Dispatcher::JustAPI(user, BRLD_GROUP_BEGIN);
       
       if (a_group->can_keys)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_keys");
       }
       
       if (a_group->can_do_all)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_do_all");
       }
       
       if (a_group->can_list)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_list");
       }
       
       if (a_group->can_geo)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_geo");
       }
       
       if (a_group->can_vector)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_vector");
       }
       
       if (a_group->can_expire)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_expire");
       }
       
       if (a_group->can_future)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_future");
       }
       
       if (a_group->can_map)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_map");
       }
       
       if (a_group->can_mmap)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_mmap");
       }
       
       if (a_group->can_publish_chan)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_publish_chan");
       }
       
       if (a_group->can_publish_clients)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_publish_clients");
       }
       
       if (a_group->can_hints)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_hints");
       }
       
       if (a_group->can_dual_exec)
       {
             user->SendProtocol(BRLD_GROUP_ITEM,  "can_dual_exec");
       }
       
       Dispatcher::JustAPI(user, BRLD_GROUP_END);
       return SUCCESS;
}

CommandGFlags::CommandGFlags(Module* Creator) : Command(Creator, "GFLAGS", 1, 1)
{
        requires = 'r';
        syntax = "<name>";
}

COMMAND_RESULT CommandGFlags::Handle(User* user, const Params& parameters) 
{
       const std::string& gname = parameters[0];
       
       std::shared_ptr<Group> a_group = Kernel->Groups->Find(gname);
       
       if (!a_group)
       {
             user->SendProtocol(ERR_INPUT, PROCESS_NULL);
             return FAILED;
       }
       
       if (a_group->GetFlags().empty() || a_group->GetFlags() == "")
       {
             user->SendProtocol(BRLD_INPUT, PROCESS_EMPTY);
             return SUCCESS;
       }
       
       user->SendProtocol(BRLD_INPUT, a_group->GetFlags());
       return SUCCESS;
}

CommandMyGroups::CommandMyGroups(Module* Creator) : Command(Creator, "MYGROUPS", 0, 0)
{

}

COMMAND_RESULT CommandMyGroups::Handle(User* user, const Params& parameters) 
{
        Dispatcher::JustAPI(user, BRLD_GROUP_BEGIN);

        GroupVector ugroups = user->GetGroups();
        
        for (GroupVector::iterator i = ugroups.begin(); i != ugroups.end(); ++i)
        {
                std::shared_ptr<Group> item = (*i);
                user->SendProtocol(BRLD_GROUP, item->GetName().c_str());
        }
        
	Dispatcher::JustAPI(user, BRLD_GROUP_END);
        
        return SUCCESS;        
}

CommandAssign::CommandAssign(Module* Creator) : Command(Creator, "GSET", 2, 2)
{
        requires = 'r';
        syntax = "<name> <user>";
}

COMMAND_RESULT CommandAssign::Handle(User* user, const Params& parameters) 
{
       const std::string& gname = parameters[0];
       const std::string& dest = parameters[1];
       
       std::shared_ptr<Group> a_group = Kernel->Groups->Find(gname);

       if (!a_group)
       {
             user->SendProtocol(ERR_INPUT, PROCESS_NULL);
             return FAILED;
       }
       
        bool exists = UserHelper::Exists(dest);

        if (!exists)
        {
               user->SendProtocol(ERR_INPUT2, ERR_NO_FLAGS, PROCESS_NULL);
               return FAILED;
        }
        
        const std::string& usergrups = dest + "/groups";
        Args grouplist = STHelper::HKeys(usergrups);
        
        for (Args::iterator i = grouplist.begin(); i != grouplist.end(); i++)
        {
               std::string item = *i;
               
               if (item == gname)
               {
                     user->SendProtocol(ERR_INPUT2, ERR_INVALID_PARAM, PROCESS_ALREADY);
                     return FAILED;
               } 
        }
        
        std::shared_ptr<Group> Found = Kernel->Groups->Find(gname);
        
        if (!Found)
        {
             user->SendProtocol(ERR_INPUT2, ERR_INVALID_PARAM, PROCESS_ALREADY);
             return FAILED;
        }
        
        UserVector logins = Kernel->Clients->FindLogin(dest);

        for (UserVector::iterator usr = logins.begin(); usr != logins.end(); ++usr)
        {
             User* user_iter = *usr;
             user_iter->PushGroup(Found);
        }

        CMapsHelper::Set(usergrups, gname, convto_string(Kernel->Now()));
        
        user->SendProtocol(BRLD_INPUT, PROCESS_OK);
        return FAILED;
}

CommandGList::CommandGList(Module* Creator) : Command(Creator, "GLIST", 1, 1)
{
        requires = 'r';
        syntax = "<login>";
}

COMMAND_RESULT CommandGList::Handle(User* user, const Params& parameters) 
{
       const std::string& login = parameters[0];
       
        bool exists = UserHelper::Exists(login);

        if (!exists)
        {
               user->SendProtocol(ERR_INPUT2, ERR_NO_FLAGS, PROCESS_NULL);
               return FAILED;
        }
       
       const std::string& usergrups = login + "/groups";
       Args grouplist = STHelper::HKeys(usergrups);

       Dispatcher::JustAPI(user, BRLD_GROUP_BEGIN);

       for (Args::iterator i = grouplist.begin(); i != grouplist.end(); i++)
       {
                std::string gname = *i;
                user->SendProtocol(BRLD_GROUP_ITEM, gname.c_str());
       }
       
       Dispatcher::JustAPI(user, BRLD_GROUP_END);
       return SUCCESS;
}

CommandGReset::CommandGReset(Module* Creator) : Command(Creator, "GRESET", 1, 1)
{
        requires = 'r';
        syntax = "<name>";
}

COMMAND_RESULT CommandGReset::Handle(User* user, const Params& parameters) 
{
        const std::string& gname = parameters[0];
       
        if (gname == "default")
        {
             user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
             return FAILED;
        }

        std::shared_ptr<Group> Found = Kernel->Groups->Find(gname);

        if (!Found)
        {
             user->SendProtocol(ERR_INPUT2, ERR_INVALID_PARAM, PROCESS_NULL);
             return FAILED;
        }

        if (!Kernel->Groups->ResetFlags(gname))
        {
             user->SendProtocol(ERR_INPUT, PROCESS_NULL);
             return FAILED;
        }

        user->SendProtocol(BRLD_INPUT, PROCESS_OK);
        return FAILED;
}

CommandUnAssign::CommandUnAssign(Module* Creator) : Command(Creator, "GDEL", 2, 2)
{
        requires = 'r';
        syntax = "<name> <user>";
}

COMMAND_RESULT CommandUnAssign::Handle(User* user, const Params& parameters) 
{
       const std::string& gname = parameters[0];
       const std::string& dest = parameters[1];

       std::shared_ptr<Group> a_group = Kernel->Groups->Find(gname);

       if (!a_group)
       {
             user->SendProtocol(ERR_INPUT, PROCESS_NULL);
             return FAILED;
       }
       
       bool exists = UserHelper::Exists(dest);

       if (!exists)
       {
               user->SendProtocol(ERR_INPUT2, ERR_NO_FLAGS, PROCESS_NULL);
               return FAILED;
       }
       
       const std::string& usergrups = dest + "/groups";
       Args grouplist = STHelper::HKeys(usergrups);

       bool found = false;
       
       for (Args::iterator i = grouplist.begin(); i != grouplist.end(); i++)
       {
               std::string item = *i;

               if (item == gname)
               {
                    found = true;
                    break;
               } 
       }
       
       if (!found)
       {
             user->SendProtocol(ERR_INPUT2, ERR_INVALID_PARAM, PROCESS_NULL);
             return FAILED;
       }

        std::shared_ptr<Group> Found = Kernel->Groups->Find(gname);

        if (!Found)
        {
             user->SendProtocol(ERR_INPUT2, ERR_INVALID_PARAM, PROCESS_ALREADY);
             return FAILED;
        }
        
        UserVector logins = Kernel->Clients->FindLogin(dest);

        for (UserVector::iterator o = logins.begin(); o != logins.end(); ++o)
        {
             User* user_iter = *o;
             user_iter->RemoveGroup(Found);
        }
        
        CMapsHelper::Del(usergrups, gname);
        user->SendProtocol(BRLD_INPUT, PROCESS_OK);
        return FAILED;
}