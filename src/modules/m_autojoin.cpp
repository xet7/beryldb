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
#include "exit.h"

class ModuleAutojoin : public Module
{ 
  
  private:
      
       std::vector<std::string> chans;
  
  public:
 
        ModuleAutojoin() 
        {
        
        }
        
        void OnPostConnect(User* user)
        {
             for (std::vector<std::string>::iterator i = this->chans.begin(); i != this->chans.end(); ++i)
             {
                  std::string channel = *i;
                  
                  LocalUser* localuser = IS_LOCAL(user);

                  if (localuser)
                  {
                        Channel::JoinUser(false, localuser, channel, true);
                  }
             }
        }
        
        void ConfigReading(config_status& status)
        {
               std::string reading = Kernel->Config->GetConf("autojoin")->as_string("chans");               
               
               engine::comma_node_stream CMD(reading);
               std::string server;

               while (CMD.items_extract(server))
               {
                     if (!Kernel->Engine->ValidChannel(server))
                     {
                            bprint(ERROR, "Invalid channel: %s", server.c_str());
                            Kernel->Exit(EXIT_CODE_CONFIG, true, true);
                     }
                     
                     chans.push_back(server); 
               }
               
               if (chans.empty())
               {
                     bprint(ERROR, "No channels provided in: <autojoin> config tag.");
                     Kernel->Exit(EXIT_CODE_CONFIG, true, true);
               }
        }
        

        Version GetDescription()
        {
                return Version("Provides AUTOJOIN module.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleAutojoin)
        
