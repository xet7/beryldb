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
        
        /* Destructor, deletes mhandler. */
        
        ~MultiMapHandler();

        /* Static constructor. */

        static std::shared_ptr<MultiMapHandler> Create(const std::string& load);
        
        /* Removes all items in this->mhandler */
        
        void Erase();
        
        /* 
         * Adds a new item to the mhandler.
         * 
         * @parameters:
	 *
	 *         · string	: Key to add.
	 *         · string	: Value to add.
         */            
         
        void Add(const std::string& key, const std::string& value);

        /* 
         * Removes an item from mhandler.
         * 
         * @parameters:
	 *
	 *         · string	: Key to be removed.
         */            
         
        void Remove(const std::string& key);
        
        /* Dumps data. */
                
        void Dump();

        /* 
         * Obtains a key from mhandler.
         * 
         * @parameters:
	 *
	 *         · string	: Key to get.
	 * 
         * @return:
 	 *
         *         · string	: Value obtained.
         */    
         
        std::string Get(const std::string& key);

        /* 
         * Counts repeats in a multimap.
         * 
         * @parameters:
	 *
	 *         · string	: Key to count.
	 * 
         * @return:
 	 *
         *         · uint	: Counter.
         */            
         
        unsigned int Repeats(const std::string& key);

        /* 
         * Determines if a given key exists.
         * 
         * @parameters:
         *
         *         · string	: Item to check.
         * 
         * @return:
         *
         *        - bool
         *       
         *          · true	: Item is in map.
         *          · false     : Item not in map.
         */

        bool Exists(const std::string& word);

        /* 
         * Deletes a key.
         * 
         * @parameters:
         *
         *         · string    : Key to delete.
         * 
         * @return:
         *
         *        - bool
         *
         *          · true     : Key has been deleted.
         *          · false    : Unable to delete key.
         */    
                 
        bool Delete(const std::string& key);

        /* 
         * Finds a key in the vector.
         * 
         * @parameters:
         *
         *         · string    :  Key to find.
         * 
         * @return:
         *
         *         · vector    :  Found keys.
         */    
         
        std::vector<std::string> Find(const std::string& key);
        
        std::string as_string();

        /* 
         * Obtain values from a map.
         * 
         * @return:
 	 *
         *         · vector    : List of vectors.
         */            
         
        std::vector<std::string> GetValues();

        /* 
         * Obtain all values in mhandler.
         * 
         * @return:
 	 *
         *         · multimap  : All values.
         */    
         
        DualMMap GetAll();

        std::vector<std::string> GetVals(const std::string& key);

        /* 
         * Counts items in multimap.
         * 
         * @return:
 	 *
         *         · uint      : Counter.
         */    
         
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
        
        
        

        
        

