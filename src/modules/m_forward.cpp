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

#include <algorithm>

#include "beryl.h"
#include "exit.h"
#include "engine.h"

class ModuleForward : public Module
{
  private:
  
       std::map<std::string, std::string> forwards;
       
  public:
 
        ModuleForward()
        {
        
        }
        
        ModuleResult OnUserPreJoin(LocalUser* user, Channel* chan, const std::string& cname)
        {
                std::string target = cname;
                std::transform(target.begin(), target.end(), target.begin(), ::toupper);

                std::map<std::string, std::string>::iterator finding = forwards.find(target);
                
                if (finding != forwards.end())
                {
                        Channel::JoinUser(false, user, finding->second, true);
                        return MOD_RES_STOP;
                }
                
                return MOD_RES_SKIP;
        }
        
        void OnUnloadModule(Module* module)
        {
                this->forwards.clear();
        }
        
        void ConfigReading(config_status& status)
        {
                MultiTag tags = Kernel->Config->GetTags("forward");

                for (config_iterator i = tags.first; i != tags.second; ++i)
                {
                        config_rule* tag = i->second;
                        
                        std::string from = tag->as_string("from");
                        std::string to = tag->as_string("to");
                        
                        if (!Kernel->Engine->ValidChannel(from))
                        {
                              bprint(ERROR, "Invalid channel: %s", from.c_str());
                              Kernel->Exit(EXIT_CODE_CONFIG, true, true);
                        }
                        
                        if (!Kernel->Engine->ValidChannel(to))
                        {
                              bprint(ERROR, "Invalid channel: %s", to.c_str());
                              Kernel->Exit(EXIT_CODE_CONFIG, true, true);
                        }
                        
                        std::transform(from.begin(), from.end(), from.begin(), ::toupper);
                        this->forwards.insert(std::make_pair(from, to));
                }
        }
        

        Version GetDescription()
        {
                return Version("Provides Forward module.", VF_BERYLDB|VF_OPTCOMMON);
        }
};

MODULE_LOAD(ModuleForward)
        
