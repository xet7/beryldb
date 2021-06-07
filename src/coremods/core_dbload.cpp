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
#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "managers/lists.h"
#include "managers/user.h"
#include "managers/settings.h"

#include "helpers.h"
#include "subscription.h"
#include "settings.h"

namespace
{
     /* Loads auto join channels from TABLE_AUTOJOIN. */
     
     void Autojoin(User* user)
     {
              VectorTuple listvector = ListHelper::Get(TABLE_AUTOJOIN, user->login);
              Args chans = std::get<1>(listvector);

              LocalUser* localuser = IS_LOCAL(user);

              for (Args::iterator i = chans.begin(); i != chans.end(); i++)
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
             const std::string level = STHelper::Get("notify", user->login);
             
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
                    
                 if (!Kernel->Sets->AsBool("autojoin"))
                 {
                     return;
                 }

                 /* Users joins channels based on the TABLE_AUTOJOIN */
                 
                 ListHelper::Add(TABLE_AUTOJOIN, memb->user->login, memb->chan->name);
        }

        void OnUserPart(Subscription* memb, DiscardList& excepts)
        {		
                 if (!Kernel->Sets->AsBool("autojoin"))
                 {
                      return;
                 }

                 ListHelper::Delete(TABLE_AUTOJOIN, memb->user->login, memb->chan->name);
        }

        /*
         * This overload will perform some actions based on the user's preferences,
         * this may be auto joins, flags assigned to the user and some configuration
         * settings.
         */
         
        void OnPostConnect(User* user)
        {       
              user->SendProtocol(BRLD_CONNECTED, Daemon::Format("%s %s", Kernel->GetVersion(false).c_str(), convto_string(Kernel->Now()).c_str()));
              
              /* Verify if user has a identical loggin logged. */
              
              std::shared_ptr<Session> session = Kernel->Logins->Sessions->Find(user->login);
              
              if (session)
              {
                    Kernel->Logins->Sessions->Attach(user, user->login, session->rawflags);
                    user->SendProtocol(BRLD_YOUR_FLAGS, user->login, session->rawflags.c_str());
                    LoadNotify(user);
                    return;
              }
              
              const std::string& flags = UserHelper::FindAdmin(user->login);    
              Kernel->Logins->Sessions->Attach(user, user->login, flags);
              LoadNotify(user);
              
              /* All users should be notified about their flags. */
              
              user->SendProtocol(BRLD_YOUR_FLAGS, user->login, flags.c_str());
              
              /* User will not join chans */
              
              if (!Kernel->Sets->AsBool("autojoin"))
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
        }

        Version GetDescription() 
        {
                return Version("Provides services to the CoreDatabase.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleCoreDB)



