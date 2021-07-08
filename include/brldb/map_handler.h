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

        /* Destructor */
        
        ~MapHandler();
        
        /* Static constructor. */

        static std::shared_ptr<MapHandler> Create(const std::string& load);
        
        /* Remove all elements in map. */
        
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
        
        /* Removes all items */
        
        void Dump();
        
        /* 
         * Calculates the length of a string.
         * 
         * @parameters:
	 *
	 *         · key: Key to calculate.
	 * 
         * @return:
 	 *
         *         · uint: Length.
         */    

        unsigned int Strlen(const std::string& key);

        /* 
         * Obtains a value associated with a key.
         * 
         * @parameters:
	 *
	 *         · key: Key to find.
	 * 
         * @return:
 	 *
         *         · std::string: value.
         */            
         
        std::string Get(const std::string& key);

        /* 
         * Determines if a given key exists.
         * 
         * @parameters:
	 *
	 *         · word: Word to check.
	 * 
         * @return:
 	 *
         *         · True: Word is in map.
         *         · False: Word not in map.
         */            
         
        bool Exists(const std::string& word);

        /* 
         * Deletes a key.
         * 
         * @parameters:
	 *
	 *         · key: Key to delete.
	 * 
         * @return:
 	 *
         *         · True: Key has been deleted.
         *         · False: Unable to delete key.
         */            
         
        bool Delete(const std::string& key);

        /* 
         * Obtains all values in a map.
	 * 
         * @return:
 	 *
         *         · DualMap: A key-value map.
         */    
         
        DualMap GetAll();

        /* Returns map as string. */
                
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

        void WildRemove(const std::string& key);

        /* 
         * Get values from a map.
	 * 
         * @return:
 	 *
         *         · std::vector: Vector of strings.
         */    
         
        std::vector<std::string> GetValues();
        
        MapMap& GetList()
        {
              return this->mhandler;
        }
        
        HANDLER_MSG GetLast()
        {
              return this->LastMsg;
        }
};
        
        
        

        
        

