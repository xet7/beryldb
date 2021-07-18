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
 
class ExportAPI Settings
{
   private:
          
        std::map<std::string, std::string> defaults;
        
        std::map<std::string, std::string> SetMap;
        
   public:
        
        Settings();    

        /* 
         * Sets a key to SetMap.
         * 
         * @parameters:
	 *
	 *         · key: Key to define.
	 *         · value: String value to assign.
         */    
                 
        void Set(const std::string& key, const std::string& value);
        
        /* 
         * Returns a particular setting found in SetMap.
         * 
         * @parameters:
	 *
	 *         · key: Key to lookup.
	 * 
         * @return:
 	 *
         *         · std::string: value, as keyed in this->SetMap.
         */    
        
        std::string Get(const std::string& key);

        /* 
         * Returns configuration setting as boolean.
         * 
         * @parameters:
	 *
	 *         · key: Key to lookup.
	 * 
         * @return:
 	 *
         *         · bool: Returning value in value.
         */            
         
        bool AsBool(const std::string& key);
        
        /* Sets default configuration values */

        void SetDefaults();
        
        /* Load data */
        
        void Load();
};

