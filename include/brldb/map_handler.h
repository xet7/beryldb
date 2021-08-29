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
	 *         · string	: Key to add.
	 *         · strnig	: Value to add.
         */            
         
        void Add(const std::string& key, const std::string& value);
        
        /* 
         * Removes an item from map.
         * 
         * @parameters:
	 *
	 *         · string	: Key to remove.
         */    
                 
        void Remove(const std::string& key);
        
        /* Removes all items */
        
        void Dump();
        
        /* 
         * Calculates the length of a string.
         * 
         * @parameters:
	 *
	 *         · string	: Key to calculate.
	 * 
         * @return:
 	 *
         *         · uint	: Length.
         */    

        unsigned int Strlen(const std::string& key);

        /* 
         * Obtains a value associated with a key.
         * 
         * @parameters:
	 *
	 *         · string	: Key to find.
	 * 
         * @return:
 	 *
         *         · string	: value.
         */            
         
        std::string Get(const std::string& key);

        /* 
         * Determines if a given key exists.
         * 
         * @parameters:
	 *
	 *         · string	: Item to check.
	 * 
         * @return:
 	 *
         *         · bool/true  : Item is in map.
         */            
         
        bool Exists(const std::string& word);

        /* 
         * Deletes a key.
         * 
         * @parameters:
	 *
	 *         · string	: Key to delete.
	 * 
         * @return:
 	 *
 	 *       -bool
 	 *      
         *         · true       : Key has been deleted.
         *         · false      : Unable to delete key.
         */            
         
        bool Delete(const std::string& key);

        /* 
         * Obtains all values in a map.
	 * 
         * @return:
 	 *
         *         · DualMap	  : A key-value map.
         */    
         
        DualMap GetAll();

        /* Returns map as string. */
                
        std::string as_string();

        /* 
         * Counts item in map.
         * 
         * @return:
 	 *
         *         · unsigned int  : Counter.
         */    
         
        unsigned int Count()
        {
             this->LastMsg = HANDLER_MSG_OK;
             return this->mhandler.size();
        }

        /* 
         * Get values from a map.
	 * 
         * @return:
 	 *
         *         · vector	: Vector of strings.
         */    
         
        std::vector<std::string> GetValues();

        /* 
         * Returns all items in mhandler.
	 * 
         * @return:
 	 *
         *         · MapMap	: All items pending.
         */            
         
        MapMap& GetList()
        {
              return this->mhandler;
        }
        
        HANDLER_MSG GetLast()
        {
              return this->LastMsg;
        }
};
        
        
        

        
        

