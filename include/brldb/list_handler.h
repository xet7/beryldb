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

class ExportAPI ListHandler
{
  private:
    
        ListMap mhandler;

        HANDLER_MSG LastMsg;
     
  public:
  
        /* Constructor */
         
        ListHandler();
         
        /* Static constructor. */
         
        static std::shared_ptr<ListHandler> Create(const std::string& load);
     
        void Dump();
         
        /* Removes all elements. */
         
        void Erase();

        void PopFront();

        bool Exist(const std::string& key);

        unsigned int Repeats(const std::string& word);

        std::string Index(unsigned int pos);
        
        /*  Removes last element in mhandler. */

        void PopBack();
         
        /* 
         * Removes all matching keys for given element.
         * 
         * @parameters:
	 *
	 *         · string key: Key to remove.
         */             
         
        void PopAll(const std::string& key);
     
         /* Returns mhandler as string. */
         
        std::string as_string();

        /* 
         * Adds a new item to the end of the list (push_back).
         * 
         * @parameters:
	 *
	 *         · string: Key to add.
         */         
         
        void Add(const std::string& key);

        /* 
         * Removes a key from a list.
         * 
         * @parameters:
	 *
	 *         · string: Key to remove.
         */         
         
        void Remove(const std::string& key);

        /* 
         * Counts items in the mhandler list.
         * 
         * @return:
 	 *
         *         · uint: List size.
         */    
              
        unsigned int Count()
        {
                this->LastMsg = HANDLER_MSG_OK;
                return this->mhandler.size();
        }
         
        void Reverse()
        {
                this->LastMsg = HANDLER_MSG_OK;                
                this->mhandler.reverse();
        }
         
        /* 
         * Resizes a given list.
         * 
         * @parameters:
	 *
	 *         · size: Shrink to size.
	 * 
         */    
         
        void Resize(unsigned int size)
        {
               this->LastMsg = HANDLER_MSG_OK;
               this->mhandler.resize(size);
        }

         /* Sorts list. */
         
        void Sort()
        {
               this->LastMsg = HANDLER_MSG_OK;               
               this->mhandler.sort();
        }

        /* 
         * Finds an item in list and returns a vector.
         * 
         * @parameters:
	 *
	 *         · string (wildcard): key to lookup.
	 * 
         * @return:
 	 *
         *         · vector: String vector containing all matched keys.
         */              
         
        std::vector<std::string> Find(const std::string& key);
         
        /* 
         * Returns a copy of this->mhandler.
         * 
         * @return:
 	 *
         *         · ListMap list.
         */         
         
        ListMap& GetList()
        {
                return this->mhandler;
        }

        HANDLER_MSG GetLast()
        {
             return this->LastMsg;
        }
        
};

