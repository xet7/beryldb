/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
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
}

class ModuleCoreDB : public Module
{

 public:

        ModuleCoreDB()
        {
              Kernel->Lock = false;
        }
        
        void OnEveryTwoSeconds(time_t current)
        {
               if (Kernel->Lock)
               {
                     return;
               }
             
               if (Kernel->Interval != 2)
               {
                     Kernel->Interval = 2;
               }
        }
        
        void OnEveryHalfMinute(time_t current)
        {
               if (Kernel->Lock)
               {
                      return;
               }
               
               /* We sleep, as there is no activity in the system. */
               
               if (!Kernel->Clients->GetLocals().size())
               {
                      Kernel->Interval = 1000;
                      Kernel->Lock = true;
               }
        }

        void OnUserJoin(Subscription* memb, bool sync, bool created_by_local, DiscardList& excepts)
        {
                 /* Checks if this user is joining from the autojoin() function. */
                 
                 if (!memb || memb->from_connect)
                 {
                      return;
                 }
                 
                 /* Will only insert chans to the table if AutoJoin is enabled. */
                    
                 if (!Kernel->Config->AutoJoin)
                 {
                     return;
                 }

                 /* Users joins channels based on the TABLE_AUTOJOIN */
                 
                 ListHelper::Add(TABLE_AUTOJOIN, memb->user->login, memb->chan->name);
        }

        void OnUserPart(Subscription* memb, DiscardList& excepts)
        {		
                 if (!Kernel->Config->AutoJoin)
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
              /* This user may bring activity to the system. */
              
              if (Kernel->Lock)
              {
                   Kernel->Lock = false;
              }

              user->SendProtocol(BRLD_CONNECTED, convto_string(Kernel->Now()).c_str());

              const std::string& flags = UserHelper::FindAdmin(user->login);    
              Kernel->Logins->Sessions->Attach(user, user->login, flags);
              
              /* We should notify clients they have flags, only they do have them. */
              
              if (!flags.empty())
              {
                    user->SendProtocol(BRLD_YOUR_FLAGS, user->login, flags.c_str());
              }
              
              /* User will not join chans */
              
              if (!Kernel->Config->AutoJoin)
              {
                    return;
              }
              
              /* Iterates over the user's channels. */
              
              Autojoin(user);
        }
        
        /* Loads configuration from core database. */
        
        void Initialize()
        {
              std::string result = STHelper::Get("conf", "allowchans");
              Kernel->Config->Sets->Set("allowchans", result, true);

              result = STHelper::Get("conf", "syntaxhints");
              Kernel->Config->Sets->Set("syntaxhints", result, true);

              result = STHelper::Get("conf", "autojoin");
              Kernel->Config->Sets->Set("autojoin", result, false);
              
              result = STHelper::Get("conf", "chansync");
              Kernel->Config->Sets->Set("chansync", result, false);
              
        }

        Version GetDescription() 
        {
                return Version("Provides services to the CoreDatabase.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleCoreDB)



