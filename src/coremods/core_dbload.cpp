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
#include "engine.h"

#include "managers/maps.h"
#include "managers/user.h"
#include "managers/settings.h"
#include "managers/expires.h"

namespace
{
     void LoadGroups(User* user)
     {		
             User* InUse = Kernel->Clients->FirstLogin(user->login);
             
             if (InUse && InUse != user)
             {
                   user->SetGroups(InUse->GetGroups());
                   return;
             }
             
             const std::string& usergrups = user->login + "/groups";

             const StringVector& groups = STHelper::HList(usergrups);
             
             for (StringVector::const_iterator i = groups.begin(); i != groups.end(); i++)
             {
                       std::string channel = *i;
                       
                       std::shared_ptr<Group> group = Kernel->Groups->Find(channel);
                       
                       if (group)
                       {
                            user->PushGroup(group);
                       }
             }
     }
     
     
     /* Loads auto join channels from TABLE_AUTOJOIN. */
     
     void Autojoin(User* user)
     {
              const std::string& userchans = user->login + "/chans";
              const StringVector& chans = STHelper::HList(userchans);
              
              LocalUser* localuser = IS_LOCAL(user);

              for (StringVector::const_iterator i = chans.begin(); i != chans.end(); i++)
              {
                       std::string channel = *i;

                       Kernel->Channels->Find(channel);

                       if (!Kernel->Engine->ValidChannel(channel))
                       {
                              continue;
                       }

                       Channel::JoinUser(true, localuser, channel, true);
              }
     }
     
     void LoadNotify(User* user)
     {
             const std::string& level = STHelper::Get("notify", user->login);
             
             if (level.empty())
             {
                    return;
             }
             
             NOTIFY_LEVEL monitor = NOTIFY_DEFAULT;

             if (level == "DEFAULT")
             {
                    monitor = NOTIFY_DEFAULT;
             }
             else if (level == "VERBOSE")
             {
                    monitor = NOTIFY_VERBOSE;
             }
             else if (level == "DEBUG")
             {
                    monitor = NOTIFY_DEBUG;
             }
             else
             {
                    /* Unable to detexct notify level. */
                    
                    return;
             }

             Kernel->Notify->Add(monitor, user);
     }
}

class ModuleCoreDB : public Module
{

 public:

        ModuleCoreDB()
        {

        }
        
        void OnUserJoin(Subscription* memb, bool sync, bool created_by_local, DiscardList& excepts)
        {
                 /* Checks if this user is joining from the autojoin() function. */
                 
                 if (!memb || memb->from_connect)
                 {
                      return;
                 }
                 
                 /* Will only insert chans to the table if AutoJoin is enabled. */
                    
                 if (!Kernel->Sets->AsBool(autojoin))
                 {
                      return;
                 }

                 /* Users joins channels based on the TABLE_AUTOJOIN */
                 
                 const std::string& userchans = memb->user->login + "/chans";
                 CMapsHelper::Set(userchans, memb->chan->GetName(), convto_string(Kernel->Now()));
        }

        void OnUserPart(Subscription* memb, DiscardList& excepts)
        {		
                 if (!Kernel->Sets->AsBool(autojoin))
                 {
                      return;
                 }

                 const std::string& userchans = memb->user->login + "/chans";
                 CMapsHelper::Del(userchans, memb->chan->GetName());
        }

        /*
         * This overload will perform some actions based on the user's preferences,
         * this may be auto joins, flags assigned to the user and some configuration
         * settings.
         */
         
        void OnPostConnect(User* user)
        {       
              LoadGroups(user);
              user->SendProtocol(BRLD_CONNECTED, Daemon::Format("%s %s", Kernel->GetVersion(false).c_str(), convto_string(Kernel->Now()).c_str()));
              
              /* Verify if user has a identical loggin logged. */
              
              std::shared_ptr<Session> session = Kernel->Logins->Sessions->Find(user->login);
              
              if (session)
              {
                    Kernel->Logins->Sessions->Attach(user, user->login, session->GetFlags());
                    LoadNotify(user);
                    return;
              }
              
              const std::string& flags = UserHelper::CheckFlags(user->login);    
              Kernel->Logins->Sessions->Attach(user, user->login, flags);
              LoadNotify(user);
              
              /* User will not join chans */
              
              std::string setting = "autojoin";

              if (!Kernel->Sets->AsBool(setting))
              {
                    return;
              }
              
              /* Iterates over the user's channels. */
              
              Autojoin(user);
        }
        
        /* Loads configuration from core database. */
        
        void Initialize()
        {
                 Kernel->Sets->Load();
                 
                 const StringVector& grlist = STHelper::HList("groups");

                 for (StringVector::const_iterator i = grlist.begin(); i != grlist.end(); i++)
                 {
                        std::string name = *i;
                        std::string flags = STHelper::Get("groups", name);
                        Kernel->Groups->Add(name, flags);
                 }
                 
                 const StringVector& dblist = STHelper::HList("databases");
                 
                 unsigned int counter = 0;
                 
                 for (StringVector::const_iterator i = dblist.begin(); i != dblist.end(); i++)
                 {
                       std::string name = *i;
                       std::shared_ptr<UserDatabase> database = Kernel->Store->DBM->Find(name);
                      
                       counter++;
                       
                       if (database)
                       {
                             ExpireHelper::List(database);
                             
                             if (dblist.size() == counter)
                             {
                                  ExpireHelper::ListFutures(database, true);
                             }
                       }
                       
                 }
        }

        Version GetDescription() 
        {
                return Version("Provides services to the CoreDatabase.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleCoreDB)



