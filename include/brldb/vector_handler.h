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

#include <algorithm>
#include <vector>  

class ExportAPI VectorHandler
{
  private:
    
        Args mhandler;

        HANDLER_MSG LastMsg;
     
  public:
  
        /* Constructor */
         
        VectorHandler();
         
        /* Static constructor. */
         
        static std::shared_ptr<VectorHandler> Create(const std::string& load);

        double GetSMA();

        /* 
         * Checks whether all values are numeric or not.
	 * 
         * @return:
 	 *
         *         · True: Is numeric.
         */    
         
        bool IsNumeric();
     
        void Dump();
         
        /* Removes all elements. */
         
        void Erase();

        /* Removes element from the top of the vector. */
        
        void PopFront();

        /* 
         * Checks if a given key exists in the vector.
         * 
         * @parameters:
	 *
	 *         · string: key to check.
	 * 
         * @return:
 	 *
         *         · True: Exists.
         *         · False: Not present.
         */    
         
        bool Exist(const std::string& key);

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
         
        void EraseFrom(unsigned int from);
     
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
         * Counts items in the mhandler vector
         * 
         * @return:
 	 *
         *         · uint: Vector size.
         */    
              
        unsigned int Count()
        {
                this->LastMsg = HANDLER_MSG_OK;
                return this->mhandler.size();
        }
         
        /* 
         * Resizes a given vector.
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

        /* 
         * Finds an item in vector and returns a vector.
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

        /* Sors vector */
        
        void Sort()
        {
               this->LastMsg = HANDLER_MSG_OK;
               std::sort(this->mhandler.begin(), this->mhandler.end());
        }
        
        void Reverse();

        /* 
         * Returns a copy of this->mhandler.
         * 
         * @return:
 	 *
         *         · ListMap list.
         */         
         
        Args& GetList()
        {
                return this->mhandler;
        }

        unsigned int Repeats(const std::string& word);

        HANDLER_MSG GetLast()
        {
             return this->LastMsg;
        }
};

