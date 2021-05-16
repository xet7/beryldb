/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#pragma once

class ListHandler
{
     private:

        std::string data;

     public:

        ListHandler();

        bool Has(const std::string& entry);

        void Add(const std::string& entry)
        {
                data.append(to_bin(entry) + ",");
        }
        
        void Load(const std::string& query)
        {
            this->data = query;
        }

        std::string GetData()
        {
             return this->data;
        }
        
        std::vector<std::string> Divide();
        
        std::vector<std::string> Divide(unsigned int offset, unsigned int limit);
        
        
        void Remove(const std::string& entry, bool onlyfirst = false);

        unsigned int Count(const std::string& entry);
        
        unsigned int CountItems();
        
        
};