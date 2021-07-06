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

class ExportAPI MultiMapHandler
{
  private:

        MultiMap mhandler;
        
        HANDLER_MSG LastMsg;
        
  public:

        /* Constructor */

        MultiMapHandler();

        /* Static constructor. */

        static std::shared_ptr<MultiMapHandler> Create(const std::string& load);
        
        void Erase();
        
        void Add(const std::string& key, const std::string& value);
        
        void Remove(const std::string& key);
        
        void Dump();

        unsigned int Strlen(const std::string& key);
        
        std::string Get(const std::string& key);

        /* 
         * Counts repeats in a multimap.
         * 
         * @parameters:
	 *
	 *         · key: Key to count.
	 * 
         * @return:
 	 *
         *         · uint: Counter.
         */            
         
        unsigned int Repeats(const std::string& key);
        
        bool Exists(const std::string& word);
        
        bool Delete(const std::string& key);

        /* 
         * Finds a key in the vector.
         * 
         * @parameters:
         *
         *         · key: Key to find.
         * 
         * @return:
         *
         *         · vector: Found keys.
         */    
         
        std::vector<std::string> Find(const std::string& key);
        
        std::string as_string();
        
        unsigned int Count()
        {
             this->LastMsg = HANDLER_MSG_OK;
             return this->mhandler.size();
        }
        
        MultiMap& GetList()
        {
             return this->mhandler;
        }
        
        HANDLER_MSG GetLast()
        {
             return this->LastMsg;
        }
};
        
        
        

        
        

