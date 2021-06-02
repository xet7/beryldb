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

#pragma once

/* Settings to share across all servers. */
 
class Externalize Settings : public safecast<Settings>
{
   private:
          
        std::map<std::string, std::string> defaults;
        
        std::map<std::string, std::string> SetMap;
        
   public:
        
        Settings();    
        
        void Set(const std::string& key, const std::string& value);

        /* Obtain a configuration value. */
        
        std::string Get(const std::string& key);
        
        bool AsBool(const std::string& key);
        
        /* Sets default configuration values */

        void SetDefaults();
        
        void Load();
};

