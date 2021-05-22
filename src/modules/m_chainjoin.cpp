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

class ModuleChainJoin : public Module
{
  private:
  
       std::map<std::string, std::string> couples;
       
  public:
 
        ModuleChainJoin()
        {
        
        }
        
        ModuleResult OnUserPreJoin(LocalUser* user, Channel* chan, const std::string& cname)
        {
                std::string target = cname;
                std::transform(target.begin(), target.end(), target.begin(), ::toupper);
                
                std::map<std::string, std::string>::iterator finding = couples.find(target);
                
                if (finding != couples.end())
                {
                        Channel::JoinUser(false, user, finding->second, true);
                }
                
                return MOD_RES_SKIP;
        }
        
        void ConfigReading(config_status& status)
        {
                MultiTag tags = Kernel->Config->GetTags("chain");

                for (config_iterator i = tags.first; i != tags.second; ++i)
                {
                        config_rule* tag = i->second;
                        
                        std::string when = tag->as_string("when");
                        std::string dochan = tag->as_string("do");
                        
                        if (!Kernel->Engine->ValidChannel(when))
                        {
                              bprint(ERROR, "Invalid channel: %s", when.c_str());
                              Kernel->Exit(EXIT_CODE_CONFIG, true, true);
                        }
                        
                        if (!Kernel->Engine->ValidChannel(dochan))
                        {
                              bprint(ERROR, "Invalid channel: %s", dochan.c_str());
                              Kernel->Exit(EXIT_CODE_CONFIG, true, true);
                        }
                        
                        std::transform(when.begin(), when.end(), when.begin(), ::toupper);
                        this->couples.insert(std::make_pair(when, dochan));
                }
        }
        

        Version GetDescription()
        {
                return Version("Provides ChanJoin module.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleChainJoin)
        
