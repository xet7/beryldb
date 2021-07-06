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

class ExportAPI MapHandler
{
  private:

        MapMap mhandler;
        
        HANDLER_MSG LastMsg;
        
  public:

        /* Constructor */

        MapHandler();

        /* Static constructor. */

        static std::shared_ptr<MapHandler> Create(const std::string& load);
        
        void Erase();

        /* 
         * Adds an item to the map.
         * 
         * @parameters:
	 *
	 *         · key: Key to add.
	 *         · value: Value to add.
         */            
         
        void Add(const std::string& key, const std::string& value);
        
        /* 
         * Removes an item from map.
         * 
         * @parameters:
	 *
	 *         · key: Key to remove.
         */    
                 
        void Remove(const std::string& key);
        
        void Dump();

        unsigned int Strlen(const std::string& key);
        
        std::string Get(const std::string& key);
        
        bool Exists(const std::string& word);
        
        bool Delete(const std::string& key);
        
        std::string as_string();

        /* 
         * Counts item in map.
         * 
         * @return:
 	 *
         *         · unsigned int: Counter.
         */    
         
        unsigned int Count()
        {
             this->LastMsg = HANDLER_MSG_OK;
             return this->mhandler.size();
        }
        
        MapMap& GetList()
        {
             return this->mhandler;
        }
        
        HANDLER_MSG GetLast()
        {
             return this->LastMsg;
        }
};
        
        
        

        
        

